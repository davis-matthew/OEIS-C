/*
 * A202267  Numbers in which all digits are noncomposites (1, 2, 3, 5, 7) or
 *          0.
 * https://oeis.org/A202267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 6480, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A202267: numbers whose decimal digits are non‑composite
   (0,1,2,3,5,7).  The term a(n) is obtained by writing n−1 in base‑6
   and mapping each base‑6 digit d∈{0,…,5} to c(d)∈{0,1,2,3,5,7}.  The
   resulting digit string is interpreted in base‑10.  For n=1 we have
   a(1)=0. */

#include <stddef.h>

static const unsigned char map[6] = {0, 1, 2, 3, 5, 7};

seq_t a(idx_t n)
{
    /* Special case for the first term */
    if (n == 1)
        return 0;

    idx_t tmp = n - 1;        /* n ≥ 2 ensures no underflow */
    /* digits in reverse order (least significant first) */
    unsigned char digits[64];
    size_t len = 0;

    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 6);
        tmp /= 6;
    }
    /* If n > 1, len will be at least 1 */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        /* Process digits from most significant to least */
        unsigned char d = digits[len - 1 - i];
        unsigned char mapped = map[d];
        result = result * 10 + mapped;
    }
    return result;
}
