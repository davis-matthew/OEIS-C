/*
 * A225186  Squares in the Thue-Morse word A001285.
 * https://oeis.org/A225186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = square of the nth root word in the Thue‑Morse sequence. *
 *            The roots are obtained by starting with the four words 1, 2,
 *            121, 212 and * repeatedly applying the morphism μ defined by
 *            μ(1)=12, μ(2)=21. * The square of a root r is the concatenation
 *            rr, and the sequence a(n) * lists these square...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
/*  Node structure for the priority queue                      */
/* ------------------------------------------------------------ */
struct Node {
    char *s;          /* root string (null terminated) */
    size_t len;       /* length of the string */
};

/* ------------------------------------------------------------ */
/*  Comparison of two nodes: first by length, then lexicographic */
static int node_compare(struct Node *a, struct Node *b)
{
    if (a->len != b->len)
        return (a->len < b->len) ? -1 : 1;
    return strcmp(a->s, b->s);
}

/* ------------------------------------------------------------ */
/*  Simple binary min‑heap of Node*                              */
/* ------------------------------------------------------------ */
static struct Node **heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;

static void heap_insert(struct Node *node)
{
    if (heap_size == heap_cap) {
        size_t new_cap = (heap_cap == 0) ? 4 : heap_cap * 2;
        struct Node **tmp = (struct Node **)realloc(heap, new_cap * sizeof(struct Node *));
        if (!tmp) exit(EXIT_FAILURE);
        heap = tmp;
        heap_cap = new_cap;
    }
    size_t i = heap_size++;
    heap[i] = node;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (node_compare(heap[i], heap[p]) < 0) {
            struct Node *t = heap[i];
            heap[i]   = heap[p];
            heap[p]   = t;
            i = p;
        } else break;
    }
}

static struct Node *heap_extract_min(void)
{
    if (heap_size == 0) return NULL;
    struct Node *min = heap[0];
    heap_size--;
    if (heap_size > 0) {
        heap[0] = heap[heap_size];
        size_t i = 0;
        while (1) {
            size_t l = 2 * i + 1;
            size_t r = l + 1;
            if (l >= heap_size) break;
            size_t smallest = l;
            if (r < heap_size && node_compare(heap[r], heap[l]) < 0) smallest = r;
            if (node_compare(heap[smallest], heap[i]) < 0) {
                struct Node *t = heap[i];
                heap[i]   = heap[smallest];
                heap[smallest] = t;
                i = smallest;
            } else break;
        }
    }
    return min;
}

/* ------------------------------------------------------------ */
/*  Storage for computed values                                 */
/* ------------------------------------------------------------ */
static seq_t *saved_vals = NULL;
static size_t vals_len = 0;
static size_t vals_cap = 0;

static void ensure_vals_cap(size_t needed)
{
    if (needed <= vals_cap) return;
    size_t new_cap = (vals_cap == 0) ? 16 : vals_cap;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(saved_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    saved_vals = tmp;
    vals_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, not part of OEIS */

    size_t target = (size_t)n;

    /* initialize the priority queue with the 4 base roots
       only on the first call                                       */
    static int initialized = 0;
    if (!initialized) {
        const char *bases[4] = {"1", "2", "121", "212"};
        const size_t lens[4] = {1, 1, 3, 3};
        for (int i = 0; i < 4; ++i) {
            struct Node *node = (struct Node *)malloc(sizeof(struct Node));
            node->len = lens[i];
            node->s   = (char *)malloc(lens[i] + 1);
            memcpy(node->s, bases[i], lens[i]);
            node->s[lens[i]] = '\0';
            heap_insert(node);
        }
        initialized = 1;
    }

    /* generate new values until we have the requested term */
    while (vals_len < target) {
        /* extract the next root in the correct order */
        struct Node *root = heap_extract_min();
        if (!root) break;   /* should never happen */

        /* compute the square value: root concatenated with itself */
        seq_t val = (seq_t)0;
        for (size_t i = 0; i < root->len; ++i)
            val = val * (seq_t)10 + (seq_t)(root->s[i] - '0');
        for (size_t i = 0; i < root->len; ++i)
            val = val * (seq_t)10 + (seq_t)(root->s[i] - '0');

        /* store the value */
        ensure_vals_cap(vals_len + 1);
        saved_vals[vals_len++] = val;

        /* produce the child root μ(root) and insert it back */
        size_t child_len = 2 * root->len;
        char *child_str = (char *)malloc(child_len + 1);
        char *p = child_str;
        for (size_t i = 0; i < root->len; ++i) {
            if (root->s[i] == '1') {
                *p++ = '1';
                *p++ = '2';
            } else {
                *p++ = '2';
                *p++ = '1';
            }
        }
        *p = '\0';

        struct Node *child = (struct Node *)malloc(sizeof(struct Node));
        child->len = child_len;
        child->s   = child_str;
        heap_insert(child);

        /* free the processed root */
        free(root->s);
        free(root);
    }

    return saved_vals[target - 1];
}
