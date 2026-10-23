/*
 * A032823  Numbers whose set of base-11 digits is {1,4}.
 * https://oeis.org/A032823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = value obtained by interpreting the binary representation
 *            of (n‑1) as a base‑11 number whose digits are 1 (for bit 0) and
 *            4 (for bit 1)
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the length L of the base‑11 digits that
       contains the nth number.  For length L there are
       2^L numbers.  The cumulative count up to length L-1
       is 2^L – 2, so we find the least L with 2^{L+1} ≥ n+2. */
    idx_t L = 1;
    idx_t bound = 4;               /* 2^{L+1} for L = 1 */
    while (bound <= n + 1) {
        bound <<= 1;
        L++;
    }

    /* offset within the block of length L numbers (0‑based) */
    idx_t offset = n + 1 - (bound >> 1);

    seq_t value = 0;
    seq_t power = 1;               /* 11^i as seq_t */

    for (idx_t i = 0; i < L; i++) {
        uint8_t bit = (offset >> i) & 1u;
        seq_t digit = bit ? (seq_t)4 : (seq_t)1;
        value += digit * power;
        power *= (seq_t)11;
    }

    return value;
}
