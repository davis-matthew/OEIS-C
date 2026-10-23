/*
 * A010126  Continued fraction for sqrt(22).
 * https://oeis.org/A010126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=4; for n≥1, a(n) = 8 if n divisible by 6, 2 if n even, 4 if
 *            n divisible by 3, 1 otherwise. This is equivalent to the
 *            multiplicative definition: a(2^e)=2, a(3^e)=4, a(p^e)=1 for p≥5
 * Range:     u8 n <= 9701, u16 n <= 9701, u32 n <= 9701, u64 n <= 9701,
 *            u128 n <= 9701, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)4;
    /* n is uint64_t, so N%6 and N%3 are inexpensive. */
    if ((n % 6ull) == 0) return (seq_t)8;
    if ((n & 1ull) == 0) /* even check via bitwise AND */
        return (seq_t)2;
    if ((n % 3ull) == 0)
        return (seq_t)4;
    return (seq_t)1;
}
