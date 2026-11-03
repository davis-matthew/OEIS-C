/*
 * A010151  Continued fraction for sqrt(73).
 * https://oeis.org/A010151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 93 terms through n = 9714 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic continued fraction for sqrt(73). a(0)=8; for n>0, a(n)=period[(n-1)%7] with period [1,1,5,5,1,1,16] */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)8;
    idx_t r = (n - 1) % 7;
    switch (r)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)5;
        case 3: return (seq_t)5;
        case 4: return (seq_t)1;
        case 5: return (seq_t)1;
        case 6: return (seq_t)16;
        default: return (seq_t)0;
    }
}
