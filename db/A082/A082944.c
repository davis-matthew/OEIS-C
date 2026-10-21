/*
 * A082944  a(n) = concatenate(n, A010888(2*n), reverse(n)), where A010888 =
 *          digital root.
 * https://oeis.org/A082944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concatenate(n, A010888(2*n), reverse(n))
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: pow10(e) returns 10^e as seq_t. */
static inline seq_t pow10(idx_t e)
{
    seq_t p = 1;
    for (idx_t i = 0; i < e; ++i)
        p *= 10;
    return p;
}

seq_t a(idx_t n)
{
    /* 1. Compute the central digit: digital root of 2*n.
       For x > 0, digital_root(x) = 1 + ((x-1) % 9),
       which is equivalently x % 9 with 0 replaced by 9. */
    seq_t two_n = (seq_t)n * 2;
    seq_t center = two_n % 9;
    if (center == 0) center = 9;

    /* 2. Count the decimal digits of n. */
    idx_t digits = 0;
    idx_t t = n;
    do {
        ++digits;
        t /= 10;
    } while (t > 0);

    /* 3. Prepare powers of 10 for shifting. */
    seq_t pow1 = pow10(digits + 1);   /* 10^(digits+1) */
    seq_t pow2 = pow1 / 10;           /* 10^digits */

    /* 4. Compute the reverse of n preserving leading zeros
       of the reversed string by iterating exactly `digits` times. */
    seq_t rev = 0;
    t = n;
    for (idx_t i = 0; i < digits; ++i) {
        rev = rev * 10 + (t % 10);
        t /= 10;
    }

    /* 5. Concatenate: n   * 10^(digits+1)
                    + center * 10^digits
                    + rev               */
    seq_t result = (seq_t)n * pow1 + center * pow2 + rev;
    return result;
}
