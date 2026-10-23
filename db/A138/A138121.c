/*
 * A138121  Triangle read by rows in which row n lists the partitions of n
 *          that do not contain 1 as a part in juxtaposed
 *          reverse-lexicographical order followed by A000041(n-1) 1's.
 * https://oeis.org/A138121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, less
 * Formula:   a(n) is the (n‑th) term of the flattened triangle * defined as
 *            follows: for m = 1, 2, … list all integer partitions * of m that
 *            contain no part equal to 1 in reverse‑lexicographic * order.
 *            Output each part of a partition separately. After all such *
 *            partitions, output p(m‑1) copies of 1, where p(...
 * Range:     u8 n <= 9751, u16 n <= 9751, u32 n <= 9751, u64 n <= 9751,
 *            u128 n <= 9751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation notes --------------------------------------------------
 * The function builds the flattened sequence lazily: on the first call it
 * generates all rows up to the required index, storing the terms in a
 * dynamic array.  Subsequent calls are answered from the stored data.
 * Partitions of a given integer are generated recursively, stored in an
 * array of structs, sorted according to reverse‑lexicographic order,
 * then the parts are appended to the global flattened array.  Only
 * integer arithmetic and standard C alloc/free are used, so the code
 * satisfies the security constraints.
 */

#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
struct Part {
    int *parts;      /* array of parts, non‑increasing order */
    size_t len;      /* number of parts */
};

static seq_t *flat      = NULL;  /* flattened sequence */
static size_t flat_cap  = 0;
static size_t flat_len  = 0;      /* number of stored elements */

static idx_t current_row = 0;    /* last row that has been generated */

/* dynamic array of Part structures used while generating a row */
static struct Part *part_list      = NULL;
static size_t part_cnt_list        = 0;
static size_t part_list_cap        = 0;

/* table of partition numbers p(k)   (k = 0 … N)                     */
static uint64_t *part_counts = NULL;
static size_t part_counts_cap = 0;

/* --------------------------------------------------------------------- */
static inline void append_flat(seq_t val)
{
    if (flat_len == flat_cap) {
        flat_cap = flat_cap ? flat_cap * 2 : 64;
        flat = (seq_t *)realloc(flat, flat_cap * sizeof(seq_t));
    }
    flat[flat_len++] = val;
}

/* --------------------------------------------------------------------- */
static void part_counts_prepare(size_t max_n)
{
    /* allocate enough room for p(0)…p(max_n) and recompute the table   */
    if (max_n >= part_counts_cap) {
        size_t new_cap = part_counts_cap ? part_counts_cap * 2 : 32;
        while (new_cap <= max_n) new_cap <<= 1;
        part_counts = (uint64_t *)realloc(part_counts, new_cap * sizeof(uint64_t));
        part_counts_cap = new_cap;
    }
    if (part_counts == NULL) {
        part_counts_cap = 32;
        part_counts = (uint64_t *)malloc(part_counts_cap * sizeof(uint64_t));
    }

    /* recompute partition numbers via the standard O(n²) DP            */
    part_counts[0] = 1;
    for (size_t i = 1; i < part_counts_cap; ++i) part_counts[i] = 0;
    for (size_t i = 1; i < part_counts_cap; ++i) {
        for (size_t j = i; j < part_counts_cap; ++j) {
            part_counts[j] += part_counts[j - i];
        }
    }
}

/* --------------------------------------------------------------------- */
static void add_partition(int *stack, size_t depth)
{
    if (part_cnt_list == part_list_cap) {
        part_list_cap = part_list_cap ? part_list_cap * 2 : 16;
        part_list = (struct Part *)realloc(part_list, part_list_cap * sizeof(struct Part));
    }
    struct Part *p = &part_list[part_cnt_list++];
    p->len   = depth;
    p->parts = (int *)malloc(depth * sizeof(int));
    memcpy(p->parts, stack, depth * sizeof(int));
}

/* --------------------------------------------------------------------- */
static void gen_partitions(int remaining, int max_part, int *stack, size_t depth)
{
    if (remaining == 0) {
        add_partition(stack, depth);
        return;
    }
    for (int part = max_part; part >= 2; --part) {
        if (part > remaining) continue;
        stack[depth] = part;
        gen_partitions(remaining - part, part, stack, depth + 1);
    }
}

/* --------------------------------------------------------------------- */
static int compare_part_rev(const void *va, const void *vb)
{
    const struct Part *a = (const struct Part *)va;
    const struct Part *b = (const struct Part *)vb;

    int i = (int)a->len - 1;
    int j = (int)b->len - 1;
    while (i >= 0 && j >= 0) {
        int va = a->parts[i];
        int vb = b->parts[j];
        if (va != vb) return (va > vb) ? -1 : 1;   /* descending */
        --i; --j;
    }
    if (i < 0 && j < 0) return 0;
    if (i < 0) return -1;  /* a shorter (prefix) is larger in reverse‑lex  */
    return 1;              /* b shorter */
}

/* --------------------------------------------------------------------- */
static void build_up_to(idx_t target)
{
    /* initialise static data on first call */
    static int initialised = 0;
    if (!initialised) {
        flat_cap = 64;
        flat = (seq_t *)malloc(flat_cap * sizeof(seq_t));
        part_counts_prepare(0);
        part_counts_cap = 1;          /* part_counts[0] is already set */
        part_counts = (uint64_t *)malloc(part_counts_cap * sizeof(uint64_t));
        part_counts[0] = 1;
        part_list_cap = 16;
        part_list     = (struct Part *)malloc(part_list_cap * sizeof(struct Part));
        initialised = 1;
    }

    while (flat_len < target) {
        /* generate next row (row number = current_row + 1)              */
        ++current_row;
        idx_t n = current_row;

        part_counts_prepare((size_t)n);   /* ensure p(0)…p(n) available   */

        /* generate all partitions of n that contain no 1                */
        part_cnt_list = 0;
        int stack[64];
        gen_partitions((int)n, (int)n, stack, 0);

        /* sort partitions in reverse‑lexicographic descending order      */
        qsort(part_list, part_cnt_list, sizeof(struct Part), compare_part_rev);

        /* output the parts of each partition                           */
        for (size_t k = 0; k < part_cnt_list; ++k) {
            const struct Part *p = &part_list[k];
            for (size_t i = 0; i < p->len; ++i) {
                append_flat((seq_t)p->parts[i]);
            }
            free(p->parts);
        }

        /* append p(n‑1) ones                                           */
        idx_t ones = part_counts[(size_t)(n - 1)];
        for (idx_t i = 0; i < ones; ++i) append_flat((seq_t)1);
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (flat == NULL) {         /* first call initialises data          */
        flat_cap = 64;
        flat = (seq_t *)malloc(flat_cap * sizeof(seq_t));
        part_counts_prepare(0);
        part_counts_cap = 1;
        part_counts = (uint64_t *)malloc(part_counts_cap * sizeof(uint64_t));
        part_counts[0] = 1;
        part_list_cap = 16;
        part_list     = (struct Part *)malloc(part_list_cap * sizeof(struct Part));
        current_row   = 0;
    }
    if (flat_len < (size_t)n) build_up_to(n);
    return flat[n - 1];  /* 1‑based offset */
}
