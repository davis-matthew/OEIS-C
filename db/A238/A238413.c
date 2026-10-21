/*
 * A238413  The irregularity of the rooted tree with Matula number n (n.>=2).
 * https://oeis.org/A238413
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   irregularity of rooted tree with Matula number n computed by
 *            constructing the tree and summing |deg(u)-deg(v)| over all edges
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ----- Prime sieve parameters ----- */
#define MAX_PRIME 1000000  /* sufficient for n <= 10 000 (and more) */

static bool    sieve_arr[MAX_PRIME + 1];
static int     primes[MAX_PRIME];
static int     prime_index[MAX_PRIME + 1];
static size_t  prime_cnt = 0;
static bool    primes_computed = false;

/* ----- Static node storage ----- */
typedef struct Node_s {
    int  parent;       /* parent index, -1 for the root */
    int  child_count;  /* number of children */
} Node;

static Node  *nodes = NULL;
static size_t node_cap = 0;
static size_t node_count = 0;

/* ----- Forward declarations of static helpers ----- */
static void   init_primes(void);
static void   ensure_node_cap(void);
static int    new_node(int parent);
static int    build_tree(idx_t n, int parent);
static seq_t  compute_irregularity(void);

/* ----- Prime sieve initialization ----- */
static void init_primes(void)
{
    if (primes_computed) return;
    primes_computed = true;

    /* sieve */
    prime_cnt = 0;
    memset(sieve_arr, 0, sizeof(sieve_arr));
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (!sieve_arr[i]) {
            prime_index[i] = (int)prime_cnt + 1; /* 1‑based index */
            primes[prime_cnt++] = i;
            if ((int64_t)i * i <= MAX_PRIME) {
                for (int j = i * i; j <= MAX_PRIME; j += i)
                    sieve_arr[j] = true;
            }
        }
    }
}

/* ----- Node array helpers ----- */
static void ensure_node_cap(void)
{
    if (node_count >= node_cap) {
        size_t new_cap = node_cap ? node_cap * 2 : 64;
        Node *tmp = (Node *)realloc(nodes, new_cap * sizeof(Node));
        if (!tmp) exit(EXIT_FAILURE);
        nodes = tmp;
        node_cap = new_cap;
    }
}

static int new_node(int parent)
{
    ensure_node_cap();
    nodes[node_count].parent = parent;
    nodes[node_count].child_count = 0;
    return (int)node_count++;
}

/* ----- Recursive construction of the Matula tree ----- */
static int build_tree(idx_t n, int parent)
{
    int root = new_node(parent);
    if (n == 1) return root;

    idx_t tmp = n;
    for (size_t i = 0; i < prime_cnt &&
                (idx_t)primes[i] * (idx_t)primes[i] <= tmp; ++i) {
        uint64_t p = (uint64_t)primes[i];
        while (tmp % p == 0) {
            tmp /= p;
            int child_num = prime_index[(int)p];
            int child = build_tree(child_num, root);
            (void)child;            /* child index is irrelevant here */
            nodes[root].child_count++;
        }
    }
    if (tmp > 1) {                    /* remaining prime factor */
        int child_num = prime_index[(int)tmp];
        int child = build_tree(child_num, root);
        (void)child;
        nodes[root].child_count++;
    }
    return root;
}

/* ----- Compute irregularity from the constructed tree ----- */
static seq_t compute_irregularity(void)
{
    seq_t total = 0;
    for (size_t i = 1; i < node_count; ++i) {    /* skip root (parent == -1) */
        int          child_idx = (int)i;
        int          parent_idx = nodes[child_idx].parent;
        seq_t        deg_child  = (seq_t)nodes[child_idx].child_count + 1;
        seq_t        deg_parent = (seq_t)nodes[parent_idx].child_count
                                 + ((nodes[parent_idx].parent == -1) ? 0 : 1);
        seq_t diff = (deg_child > deg_parent) ? (deg_child - deg_parent)
                                              : (deg_parent - deg_child);
        total += diff;
    }
    return total;
}

/* ----- Public function as required by the specification ----- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    init_primes();          /* ensure primes are ready */

    node_count = 0;         /* start fresh for this call */
    build_tree(n, -1);      /* construct the tree, root index 0 */

    return compute_irregularity();
}
