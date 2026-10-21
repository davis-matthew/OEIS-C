/*
 * A061480  n-th digit in decimal expansion of 1/n.
 * https://oeis.org/A061480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th decimal digit of 1/n. The digit is obtained by a
 *            single long‐division run of 1/n. For each digit we multiply the
 *            current remainder by 10, obtain the quotient digit, and keep the
 *            new remainder. After n steps the quotient from the last step is
 *            the desired digit
 * Range:     u8 n <= 1075, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* remainder after dividing the current integer part of the fraction by n */
    seq_t rem = 1 % (seq_t)n;         /* for n = 1 the remainder is 0 */
    seq_t digit = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* use 128‑bit arithmetic to avoid overflow (rem is < n) */
        unsigned __int128 prod = (unsigned __int128)rem * 10;
        digit = (seq_t)(prod / n);          /* current decimal digit */
        rem   = (seq_t)(prod % n);          /* new remainder */
    }
    return digit;
}
