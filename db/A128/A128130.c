/*
 * A128130  Expansion of (1-x)/(1+x^4); period 8: repeat [1,-1,0,0,-1,1,0,0].
 * https://oeis.org/A128130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = expansion of (1-x)/(1+x^4); period 8: [1,-1,0,0,-1,1,0,0]
 * Range:     i8 n <= 8191, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t tbl[8] = { (seq_t)1, (seq_t)-1, (seq_t)0, (seq_t)0,
                                 (seq_t)-1, (seq_t)1, (seq_t)0, (seq_t)0 };
    return tbl[n & 7];
}
