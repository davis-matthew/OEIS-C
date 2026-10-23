/*
 * A115953  Periodic {1,-1,0,0,1,-2,1,0,0,-1,1,-1}.
 * https://oeis.org/A115953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = {1,-1,0,0,1,-2,1,0,0,-1,1,-1}[n mod 12]
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t tbl[12] = {
        (seq_t)1, (seq_t)-1, (seq_t)0, (seq_t)0,
        (seq_t)1, (seq_t)-2, (seq_t)1, (seq_t)0,
        (seq_t)0, (seq_t)-1, (seq_t)1, (seq_t)-1
    };
    return tbl[(unsigned)(n % 12)];
}
