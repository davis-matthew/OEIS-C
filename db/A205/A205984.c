/*
 * A205984  Number of (n+1)X7 0..2 arrays with every 2X3 or 3X2 subblock
 *          having exactly two clockwise edge increases.
 * https://oeis.org/A205984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 300 * a(n-3) for n > 11. The first 11 terms are:
 *            a(1)=315357, a(2)=591336, a(3)=1599096, a(4)=7793856,
 *            a(5)=50843904, a(6)=338009472, a(7)=2260666464,
 *            a(8)=15125213376, a(9)=101283221088, a(10)=678162489984,
 *            a(11)=4537566722304
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base values for n = 1..11 */
    static const seq_t base[11] = {
        (seq_t)315357,
        (seq_t)591336,
        (seq_t)1599096,
        (seq_t)7793856,
        (seq_t)50843904,
        (seq_t)338009472,
        (seq_t)2260666464,
        (seq_t)15125213376,
        (seq_t)101283221088,
        (seq_t)678162489984,
        (seq_t)4537566722304
    };

    /* For n <= 11 just return the stored value */
    if (n <= 11ULL) {
        return base[n - 1];
    }

    /* Determine the base index (9, 10 or 11) according to n mod 3 */
    idx_t rem = n % 3ULL;
    idx_t base_index;
    if (rem == 0ULL) {
        base_index = 9ULL;
    } else if (rem == 1ULL) {
        base_index = 10ULL;
    } else {
        base_index = 11ULL;
    }

    /* Number of times 300 must be multiplied: k = (n - base_index) / 3 */
    idx_t k = (n - base_index) / 3ULL;

    seq_t result = base[base_index - 1];

    /* Compute 300^k using binary exponentiation */
    seq_t pow = (seq_t)300;
    seq_t factor = (seq_t)1;
    idx_t exp = k;
    while (exp) {
        if (exp & 1ULL) {
            factor *= pow;
        }
        pow *= pow;
        exp >>= 1ULL;
    }

    result *= factor;
    return result;
}
