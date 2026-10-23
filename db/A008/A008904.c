/*
 * A008904  a(n) is the final nonzero digit of n!.
 * https://oeis.org/A008904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = last non‑zero digit of n! (OEIS A008904)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;                     /* a(0)=a(1)=1 */

    seq_t res = 1;                           /* product modulo 10 */
    int64_t twos = 0;                        /* surplus of 2‑factors */

    for (idx_t i = 2; i <= n; ++i) {
        idx_t x = i;

        /* strip 2‑factors and count them */
        while ((x & 1u) == 0u) {
            x >>= 1;
            ++twos;
        }

        /* strip 5‑factors and cancel them with stored 2‑factors */
        while (x % 5u == 0u) {
            x /= 5u;
            --twos;
        }

        /* multiply by the remaining odd part modulo 10 */
        res = (res * (seq_t)(x % 10u)) % (seq_t)10;
    }

    /* multiply by the remaining power of 2 modulo 10 */
    if (twos > 0) {
        int r = (int)(twos % 4);
        /* 2^k modulo 10: 2,4,8,6 repeating with k≥1 */
        seq_t pow2mod = (r == 1) ? 2
                              : (r == 2) ? 4
                              : (r == 3) ? 8
                              : 6;  /* r==0 and twos>0 -> 6 */
        res = (res * pow2mod) % (seq_t)10;
    }

    return res;
}
