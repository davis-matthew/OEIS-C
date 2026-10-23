/*
 * A178407  a(n+1) = a(n)*1000 + 101^n with a(0) = 0.
 * https://oeis.org/A178407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*1000 + 101^(n-1), a(0) = 0 (equivalent to a(n) =
 *            (10^(3*n) - 101^n)/899). This implementation uses the
 *            recurrence, avoiding any intermediate that overflows the type
 *            for the range where the result fits in seq_t. For arbitrary‑
 *            precision seq_t the loop stays within the capabilities of...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur   = 0;           /* a(0) */
    seq_t pow101 = 1;          /* 101^0 */

    for (idx_t i = 0; i < n; ++i) {
        cur = cur * (seq_t)1000 + pow101;  /* a(i+1) */
        pow101 = pow101 * (seq_t)101;      /* prepare 101^(i+1) */
    }
    return cur;
}
