/*
 * A055058  Primes in A054632.
 * https://oeis.org/A055058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function returns the nth term of A055058.
 *
 * Algorithm:
 *   • Concatenate the decimal representations of the natural numbers 0,1,2,...
 *   • Keep a running sum of the digits seen so far.
 *   • Whenever the running sum is prime, record it.
 *   • If the digit that produced the sum is 0, the sum does not change;
 *     in that case the same prime would be recorded twice, so we skip
 *     recording when the digit is 0 to avoid duplicates.
 *   • Continue until the requested index n is reached.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) {                /* offset is 1; a(1) is the first term */
        return 0;
    }

    seq_t sum = 0;                /* running digit sum */
    idx_t found = 0;              /* how many primes seen so far */
    idx_t num = 0;                /* current natural number to process */

    /* buffer large enough for the decimal digits of any 64‑bit integer */
    uint8_t digits[65];           
    size_t dcnt;

    while (found < n) {
        /* extract digits of the current number into `digits` reversed */
        if (num == 0) {
            dcnt = 1;
            digits[0] = 0;
        } else {
            dcnt = 0;
            idx_t tmp = num;
            while (tmp > 0) {
                digits[dcnt++] = (uint8_t)(tmp % 10);
                tmp /= 10;
            }
        }

        /* traverse digits most significant → least significant */
        for (size_t i = dcnt; i-- > 0; ) {
            uint8_t digit = digits[i];
            sum += (seq_t)digit;
            if (digit != 0 && is_prime_big(sum)) {
                ++found;
                if (found == n) {
                    return sum;
                }
            }
        }
        ++num;                     /* next integer */
    }

    /* unreachable, but keep compiler happy */
    return sum;
}
