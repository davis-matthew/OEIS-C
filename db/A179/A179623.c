/*
 * A179623  Collatz trajectory starting at 3711.
 * https://oeis.org/A179623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 238, u64 n <= 238, u128 n <= 238,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(1) = 3711
 *            a(n) = a(n-1) / 2          if a(n-1) even
 *            a(n) = 3 * a(n-1) + 1      if a(n-1) odd
 */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)3711;   /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        if ((x % 2) == 0) {
            x = x / 2;
        } else {
            x = x * 3 + 1;
        }
    }
    return x;
}
