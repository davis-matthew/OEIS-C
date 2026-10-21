/*
 * A072166  Triangle in which first row is {1}; to get n-th row take first n
 *          numbers greater than last number in previous row which are
 *          congruent to 1 (mod n).
 * https://oeis.org/A072166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth term obtained by flattening the triangle whose
 *            rows are defined as follows: - Row 1 is {1}. - For r ≥ 2, row r
 *            consists of the first r integers greater than the last element
 *            of row r−1 that are congruent to 1 (mod r). a(n) is the nth
 *            element in the order produced by concat(row 1,...
 * Range:     u8 n <= 43, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of the triangle sequence. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    seq_t cur_last = (seq_t)1;        /* last element of the previous row */
    idx_t  count    = (idx_t)1;       /* number of terms generated so far */

    for (idx_t r = (idx_t)2; ; ++r) {
        seq_t r_seq = (seq_t)r;
        /* smallest integer > cur_last that is ≡ 1 (mod r) */
        seq_t start = ((cur_last / r_seq) + (seq_t)1) * r_seq + (seq_t)1;
        seq_t val   = start;

        for (idx_t k = (idx_t)1; k <= r; ++k) {
            ++count;
            if (count == n) {
                return val;
            }
            val += r_seq;   /* next number in the row */
        }
        cur_last = val - r_seq;   /* last number of the current row */
    }
}
