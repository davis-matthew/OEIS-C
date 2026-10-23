/*
 * A135359  a(n) is the smallest nonnegative number k such that n divides
 *          2^k-k.
 * https://oeis.org/A135359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the minimal k such that n divides 2^k - k
 * Range:     u8 n <= 75, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* safety for n==0, though n≥1 by definition */

    seq_t sn = (seq_t)n;              /* modulo value, cast to seq_t */
    seq_t mod_pow = 1 % sn;           /* 2^k mod n, starting with k = 0 */
    seq_t mod_k   = 0;                /* k mod n, starting with k = 0 */
    idx_t k = 0;                      /* current k */

    for (;; ++k)                      /* loop until match is found */
    {
        if (mod_pow == mod_k)            /* 2^k ≡ k (mod n) */
            return (seq_t)k;

        mod_pow = (mod_pow * 2) % sn;    /* update 2^(k+1) mod n */
        mod_k   = (mod_k + 1) % sn;      /* update (k+1) mod n */
    }
}
