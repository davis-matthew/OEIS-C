/*
 * A062886  Smallest multiple of 2n+1 with property that digits are odd and
 *          each digit is two more (mod 10) than the previous digit; or 0 if
 *          no such number exists.
 * https://oeis.org/A062886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest multiple of (2n+1) whose decimal digits are odd
 *            and each digit is two more (mod 10) than the previous digit; 0
 *            if no such multiple exists (this occurs when 2n+1 is divisible
 *            by 25)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: if 2n+1 is a multiple of 25, no such number exists. */
    unsigned mod = (unsigned)((uint64_t)2 * n + 1);
    if (mod % 25 == 0)
        return (seq_t)0;

    /* The five allowed odd digits in cyclic order. */
    static const unsigned char dig[5] = {1, 3, 5, 7, 9};

    /* Remainder of each candidate modulus 2n+1.  rem[i] corresponds to
       a number of current length L starting with dig[i].  */
    unsigned rem[5];
    for (int i = 0; i < 5; ++i)
        rem[i] = dig[i] % mod;

    int len = 1;   /* current length of the candidate numbers */
    while (true) {
        /* Look for a starting offset that yields a multiple of mod. */
        for (int i = 0; i < 5; ++i) {
            if (rem[i] == 0) {
                /* Build the corresponding number in seq_t. */
                seq_t val = 0;
                for (int k = 0; k < len; ++k) {
                    val = val * 10 + dig[(i + k) % 5];
                }
                return val;
            }
        }
        /* Increase the length by one digit and update the remainders. */
        ++len;
        for (int i = 0; i < 5; ++i) {
            unsigned d = dig[(i + len - 1) % 5];
            rem[i] = (rem[i] * 10 + d) % mod;
        }
    }
}
