/*
 * A063812  Growth series for fundamental group of orientable closed surface
 *          of genus 2.
 * https://oeis.org/A063812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 6*a(n-2) + 6*a(n-3) - a(n-4) for n>4; initial
 *            values a(0)=1, a(1)=8, a(2)=56, a(3)=392, a(4)=2736
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)  return (seq_t)1;
    if (n == 1)  return (seq_t)8;
    if (n == 2)  return (seq_t)56;
    if (n == 3)  return (seq_t)392;
    if (n == 4)  return (seq_t)2736;

    seq_t a0 = 1, a1 = 8, a2 = 56, a3 = 392, a4 = 2736; /* a(i-4) .. a(i) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t an = (seq_t)6 * a4 + (seq_t)6 * a3 + (seq_t)6 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
