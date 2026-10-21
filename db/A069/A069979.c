/*
 * A069979  (Sum of digits of n)^5 - (sum of digits of n^5).
 * https://oeis.org/A069979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069979 : (sum of digits of n)^5 – sum of digits of n^5
 *
 *  The value is computed exactly as follows:
 *      s   = digit_sum(n)
 *      a(n)= s^5 – digit_sum(n^5)
 *
 *  Digit sums are obtained by decimal conversion of n and of n^5.
 *  n^5 is computed with a small decimal big integer in base 1e9.
 *  All arithmetic uses only the allowed operators.
 */

#include <stdint.h>
#include <stddef.h>

static const uint32_t BASE = 1000000000u;   /* decimal base 10^9 */

/* Compute digit sum of n^exp (exp == 5) using a small big integer.  */
static seq_t digit_sum_of_power(idx_t n, int exp)
{
    /* Convert n into base BASE representation */
    uint32_t lob[128];   /* enough for every n used in tests */
    size_t len = 0;
    uint64_t tmp = n;
    do {
        lob[len++] = (uint32_t)(tmp % BASE);
        tmp /= BASE;
    } while (tmp);

    /* Multiply by n (exp-1) times to get n^exp */
    for (int k = 1; k < exp; ++k) {
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            __uint128_t prod = (__uint128_t)lob[i] * n + carry;
            lob[i] = (uint32_t)(prod % BASE);
            carry  = (uint64_t)(prod / BASE);
        }
        while (carry) {
            lob[len++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }
    }

    /* Compute digit sum of the big integer */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        uint32_t x = lob[i];
        while (x) {
            sum += x % 10;
            x /= 10;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* digit sum of n */
    seq_t sum_n = 0;
    uint64_t t = n;
    while (t) {
        sum_n += t % 10;
        t /= 10;
    }

    /* s^5 */
    seq_t s5 = 1;
    for (int i = 0; i < 5; ++i)
        s5 *= sum_n;

    /* digit sum of n^5 */
    seq_t dsum = digit_sum_of_power(n, 5);

    return s5 - dsum;
}
