/*
 * A065334  2-exponents to represent 3-smooth numbers (A065332).
 * https://oeis.org/A065334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A007814(n) * A065333(n) * In terms of the definition of
 *            the sequence: * a(n) = v2(n) if n has no prime factors other
 *            than 2 and 3, * 0 otherwise
 * Range:     u8 n <= 9353, u16 n <= 9353, u32 n <= 9353, u64 n <= 9353,
 *            u128 n <= 9353, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t temp = n;
    seq_t exp2 = 0;

    /* Count factors of 2 */
    while ((temp & 1) == 0) {
        temp >>= 1;
        exp2++;
    }

    /* Remove all factors of 3 */
    while (temp % 3 == 0) {
        temp /= 3;
    }

    /* If what remains is 1, n is 3‑smooth, else return 0. */
    return (temp == 1) ? exp2 : (seq_t)0;
}
