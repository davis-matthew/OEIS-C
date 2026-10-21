/*
 * A072674  a(n) = 3^n + 2*2^n - 3.
 * https://oeis.org/A072674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 2*2^n - 3 Recurrence: a(0)=0, a(1)=4, a(2)=14,
 *            a(n)=6*a(n-1)-11*a(n-2)+6*a(n-3) (n ≥ 3)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)14;

    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)4;   /* a(n-2) */
    seq_t a2 = (seq_t)14;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)6 * a2 - (seq_t)11 * a1 + (seq_t)6 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
