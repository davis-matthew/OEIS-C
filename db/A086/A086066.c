/*
 * A086066  a(n) = Sum_{d in D(n)} 2^d, where D(n) = set of digits of n in
 *          decimal representation.
 * https://oeis.org/A086066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = Sum_{d in D(n)} 2^d, where D(n) is the set of decimal
 *            digits of n (duplicates ignored)
 * Range:     u8 n <= 7, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Track which digits have already been seen. */
    unsigned char seen[10] = {0};
    seq_t sum = 0;
    idx_t x = n;

    /* Handle n = 0 (digit '0' only). */
    if (x == 0) {
        return (seq_t)1;  /* 2^0 = 1 */
    }

    while (x > 0) {
        int d = x % 10;        /* Current digit, 0 <= d <= 9. */
        if (!seen[d]) {
            seen[d] = 1;
            sum += ((seq_t)1 << d);  /* 2^d, computed via left shift. */
        }
        x /= 10;
    }
    return sum;
}
