/*
 * A065712  Number of 1's in decimal expansion of 2^n.
 * https://oeis.org/A065712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = count of decimal digit '1' in 2^n
 * Range:     u8 n <= 7047, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (computed by building the decimal representation of 2^n) */
seq_t a(idx_t n)
{
    /* We keep 2^k in decimal form (little‑endian digits) */
    /* For n up to about 10 000 the number has <4000 decimal digits. */
    static unsigned char digits[4000];
    int len = 1;            /* number of used digits */
    digits[0] = 1;          /* 2^0 = 1 */

    /* repeated doubling: 2^k = 2 * 2^(k‑1) */
    for (idx_t i = 1; i <= n; ++i) {
        unsigned carry = 0;
        for (int d = 0; d < len; ++d) {
            unsigned prod = digits[d] * 2 + carry;
            digits[d] = prod % 10;
            carry     = prod / 10;
        }
        if (carry) {
            digits[len++] = carry;   /* carry is always 0 or 1 */
        }
    }

    /* count the ‘1’ digits in the final decimal representation */
    seq_t count = 0;
    for (int d = 0; d < len; ++d) {
        if (digits[d] == 1) {
            ++count;
        }
    }
    return count;
}
