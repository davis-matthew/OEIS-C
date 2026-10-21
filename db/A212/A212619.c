/*
 * A212619  Sum of the distances between all unordered pairs of vertices of
 *          degree 3 in the rooted tree with Matula-Goebel number n.
 * https://oeis.org/A212619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distances between all unordered pairs of vertices
 *            of degree 3 /* in the rooted tree with Matula-Goebel number n.
 *            The function constructs the tree /* from the Matula number and
 *            computes the sum using depth and parent pointers
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_PRIME 200000         /* enough primes to factor all numbers <= 10000 */
#define MAX_FACTORS 10           /* maximum distinct prime factors for n <= 10000 */
#define MAX_NODES 20000          /* sufficient for n <= 10000 */

static bool primes_sieve_done = false;
static unsigned int primes[MAX_PRIME];
static size_t prime_cnt = 0;
static unsigned int prime_index[10001];   /* index of prime value (1‑based), 0 if not prime */

/* initialise primes up to MAX_PRIME and prime_index up to 10000 */
static void prime_sieve(void)
{
    if (primes_sieve_done) return;
    bool is_composite[MAX_PRIME];
    for (size_t i = 0; i < MAX_PRIME; ++i) is_composite[i] = false;

    for (size_t i = 2; i < MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt < (sizeof(primes)/sizeof(primes[0]))) {
                primes[prime_cnt++] = (unsigned int)i;
                if (i <= 10000)
                    prime_index[i] = (unsigned int)(prime_cnt);
            }
            if (i > MAX_PRIME / i) continue;
            for (size_t j = i * i; j < MAX_PRIME; j += i) is_composite[j] = true;
        }
    }
    primes_sieve_done = true;
}

/* factorise n into prime factors (prime values) and exponents   */
static void factor_uint32(unsigned int n,
                          unsigned int fac[MAX_FACTORS],
                          unsigned int exp[MAX_FACTORS],
                          size_t *cnt)
{
    *cnt = 0;
    unsigned int t = n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= t; ++i) {
        unsigned int p = primes[i];
        if (t % p == 0) {
            unsigned int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            fac[(*cnt)] = p;
            exp[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (t > 1) {
        fac[(*cnt)] = t;
        exp[(*cnt)] = 1;
        ++(*cnt);
    }
}

typedef struct {
    int parent;          /* -1 for root */
    int depth;           /* depth from the tree root */
    unsigned int child_cnt;
} Node;

static Node nodes[MAX_NODES];
static size_t node_cnt;

/* recursive construction of tree with Matula number m */
static int build_tree(unsigned int m, int parent)
{
    int idx = (int)node_cnt++;
    nodes[idx].parent = parent;
    nodes[idx].depth = (parent < 0) ? 0 : nodes[parent].depth + 1;
    nodes[idx].child_cnt = 0;

    if (m == 1) return idx;              /* single‑vertex tree */

    /* if m is prime, it has one child which is the tree whose number equals its index */
    if (prime_index[m] != 0) {
        unsigned int t = prime_index[m];
        int child = build_tree(t, idx);
        nodes[idx].child_cnt++;          /* the one child */
        return idx;
    }

    /* composite case: root has children corresponding to each prime factor (with multiplicity) */
    unsigned int p[MAX_FACTORS], e[MAX_FACTORS];
    size_t cnt, j;
    factor_uint32(m, p, e, &cnt);
    for (j = 0; j < cnt; ++j) {
        unsigned int times = e[j];
        int child;
        while (times--) {
            /* convert the prime factor to its prime index to obtain the child’s Matula number */
            child = build_tree(prime_index[p[j]], idx);
            nodes[idx].child_cnt++;
        }
    }
    return idx;
}

seq_t a(idx_t n)
{
    prime_sieve();                /* ensure prime tables ready */

    node_cnt = 0;
    build_tree((unsigned int)n, -1);

    /* collect indices of all vertices of degree 3 */
    int deg3[MAX_NODES];
    size_t deg3_cnt = 0;

    for (size_t i = 0; i < node_cnt; ++i) {
        int deg = nodes[i].child_cnt + (nodes[i].parent >= 0 ? 1 : 0);
        if (deg == 3) deg3[deg3_cnt++] = (int)i;
    }

    if (deg3_cnt < 2) return (seq_t)0;   /* fewer than two vertices of degree 3 */

    seq_t total = 0;

    /* compute pairwise distances via parent climbing */
    for (size_t a = 0; a < deg3_cnt; ++a) {
        int u = deg3[a];
        for (size_t b = a + 1; b < deg3_cnt; ++b) {
            int v = deg3[b];
            int uu = u, vv = v;
            /* lift depths to common level */
            while (nodes[uu].depth > nodes[vv].depth) uu = nodes[uu].parent;
            while (nodes[vv].depth > nodes[uu].depth) vv = nodes[vv].parent;
            /* ascend together until LCA found */
            while (uu != vv) {
                uu = nodes[uu].parent;
                vv = nodes[vv].parent;
            }
            int lca = uu;
            seq_t dist =
                (seq_t)nodes[u].depth + (seq_t)nodes[v].depth -
                (seq_t)2 * (seq_t)nodes[lca].depth;
            total += dist;
        }
    }
    return total;
}
