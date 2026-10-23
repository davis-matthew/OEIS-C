/*
 * A024917  a(n) = Sum_{k=2..n} k*floor(n/k).
 * https://oeis.org/A024917
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} k*floor(n/k)
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t result = 0;
    idx_t k = 2;
    while (k <= n) {
        idx_t q = n / k;               /* floor(n/k) */
        idx_t a_no = k;
        idx_t b_no;
        idx_t next;
        if (q == 1) {
            b_no = n;                  /* last block */
            next = n + 1;              /* dummy, will break */
        } else {
            next = n / q + 1;
            b_no = next - 1;
        }

        seq_t count = (seq_t)(b_no - a_no + 1);
        seq_t sum_k = (seq_t)a_no + (seq_t)b_no;          /* sum of range a_no..b_no */
        seq_t sum_range = sum_k * count / 2;             /* arithmetic series sum */

        result += (seq_t)q * sum_range;                  /* contribution */

        if (q == 1)
            break;                                       /* finished */
        k = next;                                          /* next block start */
    }
    return result;
}
