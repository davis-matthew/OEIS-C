/*
 * A136694  Final nonzero digit of n! in base 7.
 * https://oeis.org/A136694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n! / 7^v_7(n!)) mod 7, the last non‑zero digit of n! in
 *            base 7
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;
    const seq_t SEVEN = 7;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t temp = i;
        /* remove all factors of 7 from temp */
        while (temp % 7 == 0)
            temp /= 7;
        seq_t small = (seq_t)(temp % 7);
        res = (res * small) % SEVEN;
    }
    return res;
}
