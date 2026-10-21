/*
 * A206723  a(n) = 7*( ((3 + sqrt(5))/2)^n + ((3 - sqrt(5))/2)^n - 2 ).
 * https://oeis.org/A206723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*((3 + sqrt(5))/2)^n + 7*((3 - sqrt(5))/2)^n - 14
 *            Recurrence: a(0)=0, a(1)=7, a(n)=3*a(n-1)-a(n-2)+14 for n≥2
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 7;
    seq_t a0 = 0;
    seq_t a1 = 7;
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = 3 * a1 - a0 + 14;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
