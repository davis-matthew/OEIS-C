/*
 * A081179  3rd binomial transform of (0,1,0,2,0,4,0,8,0,16,...).
 * https://oeis.org/A081179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t prev = 0;
    seq_t curr = 1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = 6 * curr - 7 * prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
