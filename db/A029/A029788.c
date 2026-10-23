/*
 * A029788  Squares such that digits of sqrt(n) are not present in n^(3/2).
 * https://oeis.org/A029788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A029788: Squares of numbers k such that the decimal digits of k^2
 *  and k^3 are disjoint.  a(n) = (A029787(n))^2.
 *  (Here A029787(n) is the n-th positive integer k with no common
 *  digit between k^2 and k^3.)
 */
#include <stdint.h>
#include <stdbool.h>

static inline uint16_t digit_mask(seq_t v)
{
    uint16_t mask = 0;
    if (v == 0)
        return 1;          /* only digit 0 */
    while (v != 0) {
        int d = (int)(v % 10);
        mask |= (1u << d);
        v /= 10;
    }
    return mask;
}

/* Return the n-th term of the sequence A029788. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    /* iterate over positive integers k starting from 1 */
    for (idx_t k = 1; ; ++k) {
        seq_t kv   = (seq_t)k;
        seq_t sq   = kv * kv;          /* k^2 */
        seq_t cube = sq * kv;          /* k^3 */
        if ((digit_mask(sq) & digit_mask(cube)) == 0) {
            ++count;
            if (count == n)
                return sq;            /* a(n) = k^2 */
        }
    }
}
