/*
 * A205791  Least positive integer k such that n divides k^5-j^5 for some j in
 *          [1,k-1].
 * https://oeis.org/A205791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least positive integer k such that n divides k^5 - j^5
 *            for some j in [1, k-1]
 * Range:     u8 n <= 254, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but safe */
    seq_t N = (seq_t)n;             /* modulus */
    /* Allocate array for j^5 mod n, indices 0..n+1 (0 unused) */
    seq_t *pow = (seq_t *)malloc((n + 2) * sizeof(seq_t));
    if (!pow) return 0;             /* allocation failed */

    pow[0] = 0;
    for (idx_t j = 1; j <= n + 1; ++j) {
        seq_t val = (seq_t)j % N;
        /* compute (j mod N)^5 efficiently */
        for (int e = 0; e < 4; ++e) {
            val = (val * ((seq_t)j)) % N;
        }
        pow[j] = val;
    }

    for (idx_t k = 1; k <= n + 1; ++k) {
        seq_t target = pow[k];
        for (idx_t j = 1; j < k; ++j) {
            if (pow[j] == target) {
                free(pow);
                return (seq_t)k;
            }
        }
    }
    /* Should never reach here, but fall back to n+1 as per bound */
    free(pow);
    return (seq_t)(n + 1);
}
