/*
 * A194220  a(n) = floor(Sum_{k=1..n} frac(k/4)).
 * https://oeis.org/A194220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)/4) + floor((n+2)/4) - floor((n+6)/8)
 * Range:     u8 n <= 682, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset 1: a(1) is first term, a(0) is unused */
    if (n == 0)
        return 0;   /* value for indices below the offset is irrelevant */

    idx_t q4 = n / 4;
    idx_t r4 = n % 4;
    idx_t q8 = n / 8;
    idx_t r8 = n % 8;

    seq_t f1 = (seq_t)q4 + (r4 == 3 ? 1 : 0);      /* floor((n+1)/4) */
    seq_t f2 = (seq_t)q4 + (r4 >= 2 ? 1 : 0);      /* floor((n+2)/4) */
    seq_t f3 = (seq_t)q8 + (r8 >= 2 ? 1 : 0);      /* floor((n+6)/8) */

    return f1 + f2 - f3;
}
