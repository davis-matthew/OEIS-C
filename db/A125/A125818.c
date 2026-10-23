/*
 * A125818  a(n) = ((1 + 3*sqrt(2))^n + (1 - 3*sqrt(2))^n)/2.
 * https://oeis.org/A125818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 17*a(n-2), with a(1)=1, a(2)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; 0 is undefined */
    seq_t prev = 1;                /* a(1) */
    seq_t cur  = 1;                /* a(2) */
    for (idx_t k = 3; k <= n; ++k) {
        seq_t next = 2 * cur + 17 * prev;
        prev = cur;
        cur = next;
    }
    return cur;                    /* for n==1 or 2 cur stays 1, for n>2 it is a(n) */
}
