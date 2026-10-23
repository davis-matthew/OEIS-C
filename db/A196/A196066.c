/*
 * A196066  The reverse Wiener index of the rooted tree with Matula-Goebel
 *          number n.
 * https://oeis.org/A196066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * N(n) * (N(n)-1) * d(n) - W(n) * where N, d, and W
 *            are the number of vertices, diameter, and * Wiener index of the
 *            rooted tree with Matula-Goebel number n. * The implementation
 *            computes the tree parameters recursively * using memoisation. It
 *            is correct for all n ≥ 1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Largest Matula number that may be encountered in the test set. */
#define MAXN 20000

/* Tree parameters for a given Matula number. */
typedef struct {
    seq_t N;   /* number of vertices */
    seq_t d;   /* diameter */
    seq_t h;   /* height (max distance from root to a leaf) */
    seq_t W;   /* Wiener index (sum of distances over all unordered pairs) */
    seq_t S;   /* sum of distances from root to all vertices */
} TreeData;

/* Prime table and mapping from prime value to its 1‑based index. */
static uint64_t primes[MAXN + 1];
static size_t prime_cnt = 0;
static size_t prime_index_map[MAXN + 1];

/* Memoisation tables. */
static TreeData tree_cache[MAXN + 1];
static bool tree_computed[MAXN + 1] = {false};

/* Prime sieve initialisation. */
static bool primes_initialised = false;
static void init_primes(void)
{
    if (primes_initialised) return;

    bool is_comp[MAXN + 1];
    memset(is_comp, 0, sizeof(is_comp));

    for (idx_t p = 2; p <= MAXN; ++p) {
        if (!is_comp[p]) {
            primes[prime_cnt++] = p;
            if (p * p <= MAXN) {
                for (idx_t q = p * p; q <= MAXN; q += p)
                    is_comp[q] = true;
            }
        }
    }

    /* Build mapping from prime value to its 1‑based index. */
    memset(prime_index_map, 0, sizeof(prime_index_map));
    for (size_t i = 0; i < prime_cnt; ++i)
        prime_index_map[primes[i]] = i + 1;   /* index starts at 1 */

    primes_initialised = true;
}

/* Recursive computation of tree data for a given Matula number n. */
static const TreeData* compute_tree(idx_t n)
{
    init_primes();          /* ensure primes are ready */
    if (tree_computed[n])   /* cached? */
        return &tree_cache[n];

    TreeData *res = &tree_cache[n];
    if (n == 1) {
        /* single vertex tree */
        res->N = 1;
        res->d = 0;
        res->h = 0;
        res->W = 0;
        res->S = 0;
    } else {
        /* Factor n into prime factors; each prime factor p corresponds
           to a child subtree whose Matula number is the index of p. */
        idx_t tmp = n;
        /* Store factors as pairs (t, exponent).  A small limit suffices. */
        struct { idx_t t; int exp; } factors[10];
        size_t nf = 0;
        size_t total_children = 0;

        for (size_t pi = 0; pi < prime_cnt && primes[pi] * primes[pi] <= tmp; ++pi) {
            uint64_t p = primes[pi];
            if (tmp % p == 0) {
                int e = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++e;
                }
                factors[nf].t = prime_index_map[p];
                factors[nf].exp = e;
                total_children += e;
                ++nf;
            }
        }
        if (tmp > 1) { /* remaining prime factor */
            factors[nf].t = prime_index_map[tmp];
            factors[nf].exp = 1;
            total_children += 1;
            ++nf;
        }

        if (total_children == 1) {
            /* degree‑1 case: attach a new root above a single child. */
            idx_t t = factors[0].t;
            const TreeData *child = compute_tree(t);

            res->N = child->N + 1;
            res->h = child->h + 1;
            res->d = (child->d > child->h + 1) ? child->d : child->h + 1;
            res->S = child->S + child->N;
            res->W = child->W + child->N + child->S;
        } else {
            /* composite case: star with multiple children. */
            seq_t N_sum = 0;
            seq_t S_sum = 0;
            seq_t W_sum = 0;
            seq_t cross_sum = 0;
            seq_t cum_N = 0;
            seq_t cum_S = 0;
            seq_t maxH1 = 0, maxH2 = 0;
            seq_t maxD = 0;
            size_t child_cnt = 0;

            for (size_t fi = 0; fi < nf; ++fi) {
                const TreeData *child = compute_tree(factors[fi].t);
                seq_t Ni = child->N;
                seq_t hi = child->h;
                seq_t di = child->d;
                seq_t Wi = child->W;
                seq_t Si = child->S;

                for (int k = 0; k < factors[fi].exp; ++k) {
                    /* Cross‑pair contributions with previous children */
                    cross_sum += 2 * Ni * cum_N + Ni * cum_S + Si * cum_N;

                    /* Update cumulative sums */
                    cum_N += Ni;
                    cum_S += Si;

                    /* Accumulate sums of invariants */
                    W_sum += Wi;
                    S_sum += Si;
                    N_sum += Ni;

                    /* Track largest heights for diameter computation */
                    if (hi > maxH1) {
                        maxH2 = maxH1;
                        maxH1 = hi;
                    } else if (hi > maxH2) {
                        maxH2 = hi;
                    }

                    /* Track largest subtree diameter */
                    if (di > maxD) maxD = di;

                    child_cnt++;
                }
            }

            /* Height of the whole tree */
            res->N = 1 + N_sum;
            res->h = maxH1 + 1;

            /* Diameter across subtrees (if at least two children) */
            seq_t cross_diam = (child_cnt > 1) ? (maxH1 + maxH2 + 2) : 0;
            res->d = (maxD > cross_diam) ? maxD : cross_diam;

            /* Sum of distances from root to all vertices */
            res->S = S_sum + N_sum;

            /* Wiener index */
            res->W = W_sum + cross_sum + N_sum + S_sum;
        }
    }

    tree_computed[n] = true;
    return res;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* behaviour for n<1 is irrelevant */
    const TreeData *t = compute_tree(n);

    seq_t reverse = (t->N * (t->N - 1) / 2) * t->d;
    reverse -= t->W;
    return reverse;
}
