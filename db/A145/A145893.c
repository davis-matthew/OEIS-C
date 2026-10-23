/*
 * A145893  Triangle read by rows: T(n,k) is the number of permutations p of
 *          {1,2,...,n} such that j and p(j) are of opposite parities for k
 *          values of j (0<=k<=n).
 * https://oeis.org/A145893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle T(r,c) where T(2m,2k) =
 *            [m!*C(m,k)]^2, T(2m+1,2k) = m!·(m+1)!·C(m,k)·C(m+1,k), and
 *            T(n,k)=0 for odd k
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 108, u64 n <= 258,
 *            u128 n <= 641, big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

/* Return the entry of the triangle for row n and column k. */
static seq_t triangle_T(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if ((k & 1U) != 0) return (seq_t)0;        /* odd k gives 0 */

    idx_t j = k >> 1;                          /* k = 2·j */

    if ((n & 1U) == 0) {                       /* even n = 2·m */
        idx_t m = n >> 1;
        seq_t fact_m = factorial(m);
        seq_t comb    = binomial(m, j);
        seq_t prod    = fact_m * comb;
        return prod * prod;                    /* [m!*C(m,j)]^2 */
    } else {                                   /* odd n = 2·m+1 */
        idx_t m = n >> 1;                      /* floor(n/2) */
        seq_t fact_m  = factorial(m);
        seq_t fact_m1 = factorial(m + 1);
        seq_t comb1   = binomial(m, j);
        seq_t comb2   = binomial(m + 1, j);
        seq_t prod    = fact_m * fact_m1;
        prod = prod * comb1;
        return prod * comb2;
    }
}

/* Flattened triangle: return a(n) for the given index. */
seq_t a(idx_t idx)
{
    /* Find the row n such that n*(n+1)/2 <= idx < (n+1)*(n+2)/2. */
    idx_t n = 0;
    while (n * (n + 1) / 2 <= idx) n++;
    n--;                                         /* correct row */

    idx_t offset = n * (n + 1) / 2;
    idx_t k = idx - offset;                      /* index within the row */

    return triangle_T(n, k);
}
