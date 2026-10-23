/*
 * A121201  a(n) = 7^n+5^n-2n.
 * https://oeis.org/A121201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n + 5^n - 2*n Recurrence: a(0)=2, a(1)=10, a(2)=70,
 *            a(3)=462 a(n) = 14*a(n-1) - 60*a(n-2) + 82*a(n-3) - 35*a(n-4)
 *            (n≥4)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)70;
    if (n == 3) return (seq_t)462;

    seq_t a0 = (seq_t)2, a1 = (seq_t)10, a2 = (seq_t)70, a3 = (seq_t)462;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = 14 * a3;
        an += 82 * a1;
        an -= 60 * a2;
        an -= 35 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
