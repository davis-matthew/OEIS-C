/*
 * A122840  a(n) is the number of 0's at the end of n when n is written in
 *          base 10.
 * https://oeis.org/A122840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the number of trailing zeros in the decimal
 *            representation of n (the exponent of 10 dividing n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count trailing zeros by repeatedly dividing by 10 while the remainder
   is zero. n is an idx_t (uint64_t) so this works for all inputs
   permitted by the build. */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;          /* undefined for the sequence offset,
                                     but return 0 to be safe. */
    }
    idx_t cnt = 0;
    while (n % 10 == 0) {
        ++cnt;
        n /= 10;
    }
    return (seq_t)cnt;
}
