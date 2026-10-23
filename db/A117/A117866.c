/*
 * A117866  Number of palindromes (in base 7) below 7^n.
 * https://oeis.org/A117866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*7^((n-1)/2)-2 for odd n, * a(n) = 2*7^(n/2)-2 for even
 *            n. * Counts palindromes in base‑7 below 7^n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; undefined for n<1 */
    if (n & 1) {          /* odd n */
        seq_t exp = (seq_t)((n - 1) / 2);
        seq_t pow = big_pow((seq_t)7, exp);
        return (seq_t)8 * pow - (seq_t)2;
    } else {              /* even n */
        seq_t exp = (seq_t)(n / 2);
        seq_t pow = big_pow((seq_t)7, exp);
        return (seq_t)2 * pow - (seq_t)2;
    }
}
