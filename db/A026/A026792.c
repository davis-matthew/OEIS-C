/*
 * A026792  List of juxtaposed reverse-lexicographically ordered partitions of
 *          the positive integers.
 * https://oeis.org/A026792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 3615, u16 n <= 3615, u32 n <= 3615, u64 n <= 3615,
 *            u128 n <= 3615, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A026792 – List of juxtaposed reverse‑lexicographically ordered
   partitions of the positive integers.
   The function returns the n‑th element of that infinite list. */
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
/* Partition representation and storage structures              */
/* ------------------------------------------------------------ */
typedef struct {
    size_t len;  /* number of parts of this partition */
    int   *parts;/* array of parts, in decreasing order */
} Partition;

typedef struct {
    Partition *parts;/* array of all partitions for a given integer */
    size_t    count; /* how many partitions are stored  */
} Mdata;

/* ------------------------------------------------------------ */
/* Static memory holding intermediate results                   */
/* ------------------------------------------------------------ */
static Mdata *mdatas      = NULL;
static size_t mdatas_cap  = 0;
static size_t mdatas_len  = 0;

static idx_t *cum_parts   = NULL;   /* cumulative number of parts up to each integer */
static size_t cum_cap     = 0;
static size_t cum_len     = 0;

/* ------------------------------------------------------------ */
/* Comparison: reverse‑lexicographic order (entries compared
   from right to left, larger part considered “smaller’’).     */
/* ------------------------------------------------------------ */
static int cmp_reverse_lex(const void *a, const void *b)
{
    const Partition *pa = (const Partition *)a;
    const Partition *pb = (const Partition *)b;
    int i = (int)pa->len - 1;
    int j = (int)pb->len - 1;
    while (i >= 0 && j >= 0) {
        int va = pa->parts[i];
        int vb = pb->parts[j];
        if (va != vb) return (va > vb) ? -1 : 1;   /* descending */
        --i; --j;
    }
    if (i >= 0) return -1;   /* pa longer → greater    */
    if (j >= 0) return  1;   /* pb longer → greater    */
    return 0;
}

/* ------------------------------------------------------------ */
/* Recursive generator of all partitions of a given integer n.
   Parts are generated in standard non‑increasing order but
   we sort later into reverse‑lexicographic order.           */
/* ------------------------------------------------------------ */
static void gen_rec(int rem, int max_part, int *cur, int len,
                    Partition **buffer_ptr, size_t *cnt, size_t *cap)
{
    if (rem == 0) {
        if (*cnt == *cap) {
            *cap *= 2;
            *buffer_ptr = (Partition *)realloc(*buffer_ptr,
                                               *cap * sizeof(Partition));
        }
        Partition *buf = *buffer_ptr;
        buf[*cnt].len  = (size_t)len;
        buf[*cnt].parts = (int *)malloc(len * sizeof(int));
        memcpy(buf[*cnt].parts, cur, len * sizeof(int));
        ++(*cnt);
        return;
    }
    int upper = (rem < max_part) ? rem : max_part;
    for (int x = upper; x >= 1; --x) {
        cur[len] = x;
        gen_rec(rem - x, x, cur, len + 1, buffer_ptr, cnt, cap);
    }
}

/* ------------------------------------------------------------ */
/* Generate all partitions of integer m, sort them, and
   compute the total number of parts in all partitions.        */
/* ------------------------------------------------------------ */
static void generate_partitions_m(int m,
                                  Partition **out_parts,
                                  size_t   *out_count,
                                  idx_t    *out_total_parts)
{
    size_t cap = 4;
    Partition *buffer = (Partition *)malloc(cap * sizeof(Partition));

    int *cur = (int *)malloc(m * sizeof(int));  /* enough to hold longest partition */

    size_t cnt = 0;
    gen_rec(m, m, cur, 0, &buffer, &cnt, &cap);

    /* sort into reverse‑lexicographic order */
    qsort(buffer, cnt, sizeof(Partition), cmp_reverse_lex);

    /* count total parts */
    idx_t total = 0;
    for (size_t i = 0; i < cnt; ++i) total += (idx_t)buffer[i].len;

    free(cur); /* cur was only a temp array */

    *out_parts       = buffer;
    *out_count       = cnt;
    *out_total_parts = total;
}

/* ------------------------------------------------------------ */
/* Ensure cumulative array is initialised.                      */
/* ------------------------------------------------------------ */
static void init_cum(void)
{
    if (!cum_parts) {
        cum_cap = 4;
        cum_parts = (idx_t *)malloc(cum_cap * sizeof(idx_t));
        cum_len   = 1;
        cum_parts[0] = 0;                /* sentinel: 0 parts before any integer   */
    }
}

/* ------------------------------------------------------------ */
/* Ensure mdatas array is initialised.                          */
/* ------------------------------------------------------------ */
static void init_mdatas(void)
{
    if (!mdatas) {
        mdatas_cap = 4;
        mdatas = (Mdata *)malloc(mdatas_cap * sizeof(Mdata));
        mdatas_len = 0;
    }
}

/* ------------------------------------------------------------ */
/* Build data up to and including the integer whose cumulative
   total number of parts is at least target.                    */
/* ------------------------------------------------------------ */
static void build_until(idx_t target)
{
    init_cum();
    init_mdatas();
    while (cum_parts[cum_len - 1] < target) {
        int m = (int)cum_len;      /* next integer to process */
        Partition *parts;
        size_t     count;
        idx_t      total_parts;
        generate_partitions_m(m, &parts, &count, &total_parts);

        /* store the generated data */
        if (mdatas_len == mdatas_cap) {
            mdatas_cap *= 2;
            mdatas = (Mdata *)realloc(mdatas, mdatas_cap * sizeof(Mdata));
        }
        mdatas[mdatas_len].parts = parts;
        mdatas[mdatas_len].count = count;
        ++mdatas_len;

        /* update cumulative partial sums */
        idx_t new_cum = cum_parts[cum_len - 1] + total_parts;
        if (cum_len == cum_cap) {
            cum_cap *= 2;
            cum_parts = (idx_t *)realloc(cum_parts, cum_cap * sizeof(idx_t));
        }
        cum_parts[cum_len++] = new_cum;
    }
}

/* ------------------------------------------------------------ */
/* Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* out of bounds – not used by OEIS */

    build_until(n); /* make sure we have computed enough parts */

    /* find integer m such that cum_parts[m-1] < n <= cum_parts[m] */
    size_t m = 1;
    while (m < cum_len && cum_parts[m] < n) ++m;

    idx_t prev_sum = cum_parts[m - 1];
    idx_t offset   = n - prev_sum - 1;  /* zero‑based position within the block of integer m */

    const Partition *partitions = mdatas[m - 1].parts;
    size_t          count       = mdatas[m - 1].count;

    size_t idx = (size_t)offset;
    for (size_t i = 0; i < count; ++i) {
        if (idx < partitions[i].len)
            return (seq_t)partitions[i].parts[idx];
        idx -= partitions[i].len;
    }

    return (seq_t)0;  /* should never be reached */
}
