/*
 * A081554  a(n) = sqrt(2)*( (3+2*sqrt(2))^n - (3-2*sqrt(2))^n ).
 * https://oeis.org/A081554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=8, a(n)=6*a(n-1)-a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a0 = 0;
    seq_t a1 = 8;

    if (n == 0) return a0;
    if (n == 1) return a1;

    /* iterate up to n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = 6 * a1 - a0;  /* recurrence */
        a0 = a1;
        a1 = next;
    }
    return a1;
}
