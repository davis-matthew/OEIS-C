/*
 * A214677  a(n) = n represented in bijective base-7 numeration.
 * https://oeis.org/A214677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = bijective base-7 representation of n, interpreted as a
 *            decimal integer
 * Range:     u8 n <= 138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the digits of n in bijective base-7 (digits 1-7), least significant first */
    unsigned digits[64];          /* enough for idx_t up to 2^64-1 (≈44 digits) */
    unsigned len = 0;
    idx_t m = n;

    /* bijective base conversion: n--, digit = n % 7 + 1, n /= 7 */
    while (m > 0) {
        m -= 1;                                   /* step 1: decrement */
        unsigned digit = (unsigned)(m % 7ull) + 1; /* step 2: compute digit */
        digits[len++] = digit;                    /* store digit (least significant first) */
        m /= 7ull;                                /* step 3: shift to next position */
    }

    /* build the decimal number from most significant to least significant digit */
    seq_t result = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
    }
    return result;
}
