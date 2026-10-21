/*
 * A059796  Degrees of irreducible representations of symmetric group S_14.
 * https://oeis.org/A059796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 135, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A059796: degrees of irreducible representations of S_14.
   The function generates all partitions of 14, computes each
   hook‑length product, then dimensions as 14! divided by that
   product, sorts the list, and returns the n-th smallest dimension
   (1‑based). */

#include <stdint.h>
#include <stddef.h>

/* forward declaration of the recursive partition generator */
static void gen_partitions(int rem, int max_part, int depth, int *cur,
                           seq_t fact, seq_t *deg, int *count);

/* Build all dimensions of S_14 once */
static void compute_degrees(seq_t *deg, int *count)
{
    const int N = 14;

    /* N! as seq_t */
    seq_t fact = (seq_t)1;
    for (int i = 2; i <= N; ++i)
        fact *= (seq_t)i;

    /* array to hold the current partition (non‑increasing) */
    static int cur[15];

    /* generate all partitions and compute dimensions */
    gen_partitions(N, N, 0, cur, fact, deg, count);
}

/* Recursive generation of integer partitions of rem with largest part ≤ max_part.
   depth : current depth in cur array.
   For each complete partition, compute the hook‑length product and store the dimension. */
static void gen_partitions(int rem, int max_part, int depth, int *cur,
                           seq_t fact, seq_t *deg, int *count)
{
    for (int part = (rem < max_part ? rem : max_part); part >= 1; --part) {
        cur[depth] = part;
        if (rem == part) {        /* finished partition */
            int m = depth + 1;    /* number of parts */
            seq_t prod = (seq_t)1;
            /* compute hook product */
            for (int i = 0; i < m; ++i) {          /* rows, 0‑based */
                for (int j = 1; j <= cur[i]; ++j) { /* columns, 1‑based */
                    /* number of rows with at least j boxes (λ′_j) */
                    int lambda_prime = 0;
                    for (int k = 0; k < m; ++k)
                        if (cur[k] >= j)
                            ++lambda_prime;
                    int hook = cur[i] - j + lambda_prime - i;
                    prod *= (seq_t)hook;
                }
            }
            /* dimension = N! / hook_product */
            seq_t dim = fact / prod;
            deg[(*count)++] = dim;
        } else {
            gen_partitions(rem - part, part, depth + 1, cur, fact, deg, count);
        }
    }
}

/* Cache for pre‑computed sorted degrees */
static int deg_init = 0;
static seq_t deg_cache[200];
static int deg_n = 0;

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    if (!deg_init) {
        compute_degrees(deg_cache, &deg_n);
        /* simple bubble sort – deg_n ≤ 200 */
        for (int i = 0; i < deg_n - 1; ++i)
            for (int j = i + 1; j < deg_n; ++j)
                if (deg_cache[j] < deg_cache[i]) {
                    seq_t tmp = deg_cache[i];
                    deg_cache[i] = deg_cache[j];
                    deg_cache[j] = tmp;
                }
        deg_init = 1;
    }

    /* sequence starts at offset 1 */
    if (n < 1 || n > (idx_t)deg_n)
        return (seq_t)0;     /* out of range; not expected in tests */
    return deg_cache[n - 1];
}
