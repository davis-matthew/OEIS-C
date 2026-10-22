/*
 * A000253  a(n) = 2*a(n-1) - a(n-2) + a(n-3) + 2^(n-1).
 * https://oeis.org/A000253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) + 2^(n-1), a(0) = 0, a(1) = 1,
 *            a(2) = 4
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x0 = 0, x1 = 1, x2 = 4; /* a(k-3), a(k-2), a(k-1) */
    seq_t p = 4;                  /* 2^(k-1) */
    idx_t k;

    if (n == 0) return x0;
    if (n == 1) return x1;
    if (n == 2) return x2;

    for (k = 3; k <= n; k++) {
        seq_t next = x2 * 2 + x0 + p - x1;
        x0 = x1;
        x1 = x2;
        x2 = next;
        p = p * 2;
    }
    return x2;
}
