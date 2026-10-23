/*
 * A133933  a(n) = (1 + n * (n - 2) + (n - 1)!) mod (4*n).
 * https://oeis.org/A133933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + n*(n-2) + (n-1)!) mod (4*n)
 * Range:     u8 n <= 16, u16 n <= 542, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1; undefined for n==0 */

    seq_t mod = (seq_t)(4 * n);           /* modulus 4*n */
    seq_t n_seq = (seq_t)n;
    seq_t fact = 1;                       /* (n-1)! mod mod */

    /* Compute (n-1)! modulo mod */
    for (idx_t i = 2; i <= n - 1; ++i) {
        fact = (fact * (seq_t)i) % mod;
    }

    /* Compute 1 + n*(n-2) + fact  modulo mod */
    seq_t term = (seq_t)1 + n_seq * ((seq_t)n - 2) + fact;
    return term % mod;
}
