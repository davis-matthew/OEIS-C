/*
 * A104112  Decimal expansion of 42^42.
 * https://oeis.org/A104112
 *
 * Offset:    69    (a(69) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) returns the (n-69)-th decimal digit of 42^42 (most
 *            significant digit occurs at n = 69). For n > 137 the sequence is
 *            undefined; this implementation returns 0 for such indices
 * Range:     u8 none, u16 n <= 137, u32 n <= 137, u64 n <= 137, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* static storage for the digits of 42^42 in most‑significant‑first order */
    static seq_t digits[70];
    static idx_t     len = 0;          /* number of digits stored */

    /* initialise once: compute 42^42 via repeated decimal multiplication */
    if (len == 0) {
        /* temporary digits in least‑significant‑first order */
        seq_t temp[80];
        idx_t temp_len = 1;
        temp[0] = 1;                   /* 42^0 = 1 */

        const seq_t mult = 42;

        for (idx_t iter = 0; iter < 42; ++iter) {
            seq_t carry = 0;
            for (idx_t i = 0; i < temp_len; ++i) {
                seq_t prod = temp[i] * mult + carry;
                temp[i] = prod % 10;
                carry   = prod / 10;
            }
            while (carry > 0) {
                temp[temp_len++] = carry % 10;
                carry /= 10;
            }
        }

        len = temp_len;
        for (idx_t i = 0; i < len; ++i) {
            digits[i] = temp[len - 1 - i];   /* store most‑significant digit first */
        }
    }

    if (n < 69)        /* below the defined offset */
        return 0;

    idx_t pos = n - 69;          /* zero‑based position in the digit array */
    return (pos < len) ? digits[pos] : (seq_t)0;
}
