/*
 * A182907  Triangle read by rows: row n is the degree sequence (written in
 *          nondecreasing order) of the rooted tree with Matula-Goebel number
 *          n.
 * https://oeis.org/A182907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is the n-th term of the concatenation of the
   nondecreasing degree sequences of rooted trees with Matula–Goebel
   number 1,2,3,...  (row r is the degree sequence of the tree of
   Matula–Goebel number r).  The function builds each tree once,
   stores its degree sequence, and accumulates the total number of
   elements per row. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Comparison function used by qsort for seq_t elements. */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* --------------------------------------------------------------------- */
/*  Node structure for a tree with Matula–Goebel number n                */
/* --------------------------------------------------------------------- */
typedef struct {
    seq_t    *seq;     /* sorted degree sequence of the tree */
    size_t    len;     /* number of vertices */
    seq_t     root_deg;/* degree of the root vertex in this tree */
} Node;

/* --------------------------------------------------------------------- */
/*  Sieve data: smallest prime factor and prime → index mapping.        */
/* --------------------------------------------------------------------- */
static const size_t MAX_LIMIT = 50000;   /* sufficient for n ≤ 10 000 */
static int *spf = NULL;                  /* spf[i] = smallest prime factor of i (0 if i≤1) */
static int *prime_idx = NULL;            /* prime_idx[p] = 1‑based index of prime p */

/* --------------------------------------------------------------------- */
/*  Node cache: nodes[0] is unused.                                       */
/* --------------------------------------------------------------------- */
static Node *nodes = NULL;

/* --------------------------------------------------------------------- */
/*  Cumulative element count per row (idx_t type).                        */
/* --------------------------------------------------------------------- */
static idx_t *cumlen = NULL;   /* cumlen[0] = 0, cumlen[r] = total elements in rows 1..r */
static size_t cumlen_cap = 0;
static size_t max_row_computed = 0;

/* --------------------------------------------------------------------- */
/*  Initialisation of the sieve.                                          */
/* --------------------------------------------------------------------- */
static void init_sieve(void)
{
    if (spf != NULL) return;      /* already initialised */

    spf       = (int *)calloc(MAX_LIMIT + 1, sizeof(int));
    prime_idx = (int *)calloc(MAX_LIMIT + 1, sizeof(int));
    if (!spf || !prime_idx) exit(EXIT_FAILURE);

    int *primes = (int *)malloc(MAX_LIMIT * sizeof(int));
    if (!primes) exit(EXIT_FAILURE);
    size_t pcnt = 0;

    for (int i = 2; i <= (int)MAX_LIMIT; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = i;
            primes[pcnt] = i;
            prime_idx[i] = (int)(++pcnt); /* 1‑based index */
        }
        for (size_t j = 0; j < pcnt &&
             primes[j] <= spf[i] &&
             1LL * i * primes[j] <= (long long)MAX_LIMIT; ++j) {
            int v = i * primes[j];
            spf[v] = primes[j];
        }
    }
    free(primes);
}

/* --------------------------------------------------------------------- */
/*  Initialisation of the node cache.                                     */
/* --------------------------------------------------------------------- */
static void init_nodes(void)
{
    nodes = (Node *)calloc(MAX_LIMIT + 1, sizeof(Node));
    if (!nodes) exit(EXIT_FAILURE);
}

/* --------------------------------------------------------------------- */
/*  Compute the Node for Matula–Goebel number n.                         */
/* --------------------------------------------------------------------- */
static Node *compute_node(idx_t n)
{
    if (!spf) {
        init_sieve();
        init_nodes();
    }

    if (n > MAX_LIMIT) return NULL;    /* out of bounds for this implementation */

    Node *nd = &nodes[n];
    if (nd->seq != NULL) return nd;    /* already computed */

    /* Special case: the single‑vertex tree. */
    if (n == 1) {
        nd->root_deg = 0;
        nd->len      = 1;
        nd->seq = (seq_t *)malloc(sizeof(seq_t));
        if (!nd->seq) exit(EXIT_FAILURE);
        nd->seq[0] = (seq_t)0;
        return nd;
    }

    /* Factorise n into primes, collecting the indices of the child subtrees. */
    idx_t temp = n;
    idx_t child_ids[64];
    size_t child_cnt = 0;

    while (temp > 1) {
        int p = spf[(int)temp];          /* smallest prime factor */
        int id = prime_idx[p];           /* 1‑based index of prime p */
        child_ids[child_cnt++] = (idx_t)id;
        temp /= p;
    }

    nd->root_deg = (seq_t)child_cnt;

    /* Compute total length: 1 for root + sum of child lengths. */
    size_t total_len = 1;
    for (size_t i = 0; i < child_cnt; ++i) {
        Node *child = compute_node(child_ids[i]);
        if (!child) exit(EXIT_FAILURE);
        total_len += child->len;
    }

    /* Allocate buffer for the combined degree sequence. */
    seq_t *buffer = (seq_t *)malloc(total_len * sizeof(seq_t));
    if (!buffer) exit(EXIT_FAILURE);

    size_t pos = 0;
    for (size_t i = 0; i < child_cnt; ++i) {
        Node *child = compute_node(child_ids[i]);

        /* Copy the child's degree sequence. */
        for (size_t j = 0; j < child->len; ++j)
            buffer[pos + j] = child->seq[j];

        /* Replace the child's root degree by one more. */
        size_t found = SIZE_MAX;
        for (size_t j = 0; j < child->len; ++j) {
            if (buffer[pos + j] == child->root_deg) {
                found = j;
                break;
            }
        }
        if (found != SIZE_MAX)
            buffer[pos + found] = child->root_deg + (seq_t)1;

        pos += child->len;
    }

    /* Add the root vertex degree. */
    buffer[pos++] = nd->root_deg;
    (void)pos;   /* silence unused warning */

    /* Sort the resulting degree sequence. */
    qsort(buffer, total_len, sizeof(seq_t), cmp_seq_t);

    nd->len = total_len;
    nd->seq = buffer;
    return nd;
}

/* --------------------------------------------------------------------- */
/*  Ensure cumlen has capacity for the required indices.                 */
/* --------------------------------------------------------------------- */
static void ensure_cumlen_capacity(size_t needed)
{
    if (needed <= cumlen_cap) return;
    size_t new_cap = cumlen_cap ? cumlen_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(cumlen, new_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);
    cumlen = tmp;
    cumlen_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function.                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Initialise cumulative length cache on first call. */
    if (cumlen == NULL) {
        cumlen = (idx_t *)calloc(16, sizeof(idx_t));
        if (!cumlen) exit(EXIT_FAILURE);
        cumlen_cap = 16;
        cumlen[0] = 0;
        max_row_computed = 0;
    }

    /* Generate rows until the requested element index is reached. */
    while (cumlen[max_row_computed] < n) {
        size_t next_row = max_row_computed + 1;
        Node *node = compute_node((idx_t)next_row);
        if (!node) break;   /* safety */

        ensure_cumlen_capacity(next_row + 1);
        cumlen[next_row] = cumlen[max_row_computed] + (idx_t)node->len;
        max_row_computed = next_row;
    }

    /* Binary search to find the row that contains the nth element. */
    size_t low = 1, high = max_row_computed;
    while (low < high) {
        size_t mid = (low + high) / 2;
        if (cumlen[mid] >= n)
            high = mid;
        else
            low = mid + 1;
    }
    size_t row = low;
    idx_t offset = n - cumlen[row - 1] - 1;  /* zero‑based offset */

    return nodes[row].seq[offset];
}
