/*
 * A085278  Expansion of (1+2x)^2/((1-x^2)(1-2x)).
 * https://oeis.org/A085278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + {5 if n even, 4 if n odd}\n with a(0)=1,
 *            a(1)=6
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)6;
    }
    seq_t prev = (seq_t)1;
    seq_t curr = (seq_t)6;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t diff = (i & 1) ? (seq_t)4 : (seq_t)5; /* i odd -> 4, even -> 5 */
        seq_t next = curr * 2 + diff;
        prev = curr;
        curr = next;
    }
    return curr;
}
