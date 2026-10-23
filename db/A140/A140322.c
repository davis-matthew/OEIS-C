/*
 * A140322  a(n) = -1/6 + (-1)^n/2 + 2*4^n/3.
 * https://oeis.org/A140322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 4*a(n-3); a(0)=1, a(1)=2, a(2)=11
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return 1;
    if (n == 1U) return 2;
    if (n == 2U) return 11;

    seq_t a0 = 1;  /* a(n-3) */
    seq_t a1 = 2;  /* a(n-2) */
    seq_t a2 = 11; /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = 4 * a2 + a1 - 4 * a0;   /* a(i) based on recurrence */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
