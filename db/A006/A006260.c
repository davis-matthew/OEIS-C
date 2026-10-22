/*
 * A006260  Second-order Eulerian numbers <<n,3>>.
 * https://oeis.org/A006260
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = <<n,3>> via the second-order Eulerian recurrence <<m,k>>
 *            = (k+1)*<<m-1,k>> + (2m-1-k)*<<m-1,k-1>>, <<1,0>> = 1, <<1,k>> =
 *            0 for k > 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t e0 = 1, e1 = 0, e2 = 0, e3 = 0;
    idx_t m;
    for (m = 2; m <= n; m++) {
        e3 = (seq_t)4 * e3 + (seq_t)(2 * m - 4) * e2;
        e2 = (seq_t)3 * e2 + (seq_t)(2 * m - 3) * e1;
        e1 = (seq_t)2 * e1 + (seq_t)(2 * m - 2) * e0;
        e0 = 1;
    }
    return e3;
}
