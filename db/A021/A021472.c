/*
 * A021472  Decimal expansion of 1/468.
 * https://oeis.org/A021472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^n / 468) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(0)=0, a(1)=0, for n≥2 a(n) repeats the period {2,1,3,6,7,5}.) */
seq_t a(idx_t n)
{
    if (n < 2u)
        return (seq_t)0;
    /* the decimal expansion of 1/468 repeats every 6 digits after the first two zeros */
    static const seq_t pattern[6] = { (seq_t)2, (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)7, (seq_t)5 };
    idx_t idx = (n - 2u) % 6u;      /* 0 ≤ idx < 6 */
    return pattern[idx];
}
