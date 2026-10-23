/*
 * A036377  Floor[concatenation of seven consecutive numbers from n to n+6
 *          divided by 7].
 * https://oeis.org/A036377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor( concat(n, n+1, …, n+6) / 7 )
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    unsigned __int128 total = 0;
    for (idx_t i = 0; i < 7; ++i) {
        idx_t val = n + i;
        int d = digits(val);
        unsigned __int128 mult = (unsigned __int128)pow10(d);
        total = total * mult + (unsigned __int128)val;
    }
    unsigned __int128 q = total / 7;
    return (seq_t)q;
}
