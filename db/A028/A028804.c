/*
 * A028804  Nonsquares mod 91.
 * https://oeis.org/A028804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th positive integer that is not a quadratic residue
 *            * modulo 91 (n ≥ 1). A number is a quadratic residue modulo 91
 *            iff it is a * quadratic residue modulo both 7 and 13. The
 *            sequence lists all integers * that fail this condition
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Squares modulo 7: 0,1,2,4   ->  sq7[i] == 1 if i is a quadratic residue mod 7 */
static const unsigned char sq7[7] = {1, 1, 1, 0, 1, 0, 0};

/* Squares modulo 13: 0,1,3,4,9,10,12 -> sq13[i] == 1 if i is a quadratic residue mod 13 */
static const unsigned char sq13[13] =
    {1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1};

/* Return true if x is not a quadratic residue modulo 91. */
static inline bool is_non_square(seq_t x)
{
    unsigned long long m7  = (unsigned long long)(x % 7);
    unsigned long long m13 = (unsigned long long)(x % 13);
    return !(sq7[m7] && sq13[m13]);
}

/* OEIS sequence A028804 */
seq_t a(idx_t n)
{
    if (n == 0)                    /* offset is 1, behaviour below does not matter */
        return 0;

    idx_t     count = 0;
    seq_t     value = 1;          /* start test from 2 */

    while (count < n) {
        ++value;                  /* candidate integer to test */
        if (is_non_square(value))
            ++count;
    }
    return value;
}
