/*
 * A117980  Legendre-binomial transform of (-1)^n for p=3.
 * https://oeis.org/A117980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 727, u16 n <= 9595, u32 n <= 9595, u64 n <= 9595,
 *            u128 n <= 9595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)=0 for n≡1 (mod 3); a(3n)=a(n); a(3n+2)=3⋅a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n % 3 == 1) return (seq_t)0;
    if (n % 3 == 0) return a(n / 3);
    /* n % 3 == 2 */
    return (seq_t)3 * a((n - 2) / 3);
}
