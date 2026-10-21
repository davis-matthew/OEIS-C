/*
 * A072339  Any number n can be written (in two ways, one with m even and one
 *          with m odd) in the form n = 2^k_1 - 2^k_2 + 2^k_3 - ... + 2^k_m
 *          where the signs alternate and k_1 > k_2 > k_3 > ... >k_m >= 0;
 *          sequence gives minimal value of m.
 * https://oeis.org/A072339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = 1, a(n) = 1 + a(p - n) where p = 2^k is the smallest
 *            power of two ≥ n (i.e. p = ⌈log₂(n)⌉). Each step reduces n to
 *            p−n until a power of two is reached; the number of steps plus
 *            the final power‑of‑two term gives the minimal alternating
 *            representation length
 * Range:     u8 n <= 9619, u16 n <= 9619, u32 n <= 9619, u64 n <= 9619,
 *            u128 n <= 9619, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t len = 1;                     /* final term when n becomes a power of two */
    while ((n & (n - 1))) {            /* while n is not a power of two */
        idx_t p = 1;                    /* find smallest power of two ≥ n */
        while (p < n)
            p <<= 1;                    /* use left shift – safe for 64‑bit idx_t */
        n = p - n;                      /* next argument for recursion */
        ++len;                          /* count this term */
    }
    return len;
}
