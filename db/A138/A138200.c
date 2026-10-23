/*
 * A138200  a(n) = (14^(2*n+1) + 3^(2*n+1)) / 17.
 * https://oeis.org/A138200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138200: a(n) = (14^(2*n+1)+3^(2*n+1))/17.
   Recurrence: a(0)=1, a(1)=163, a(n)=205*a(n-1)-1764*a(n-2) for n≥2. */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 163;
    seq_t a0 = 1;
    seq_t a1 = 163;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t an = 205 * a1 - 1764 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
