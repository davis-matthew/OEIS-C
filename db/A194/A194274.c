/*
 * A194274  Concentric square numbers (see Comments lines for definition).
 * https://oeis.org/A194274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t last_even = (seq_t)0;    /* a(0) */
    seq_t last_odd  = (seq_t)1;    /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t square = ii * ii;

        if ((i & 1) == 0) {          /* even index */
            cur = square - last_even;
            last_even = cur;
        } else {                     /* odd index */
            cur = square - last_odd;
            last_odd = cur;
        }
    }
    return cur;
}
