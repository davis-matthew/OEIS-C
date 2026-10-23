/*
 * A010141  Continued fraction for sqrt(55).
 * https://oeis.org/A010141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0)=7; for n>0, a(n)=14 if (n-1)%4==3 else 2
 * Range:     u8 n <= 9617, u16 n <= 9617, u32 n <= 9617, u64 n <= 9617,
 *            u128 n <= 9617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of the simple continued fraction of sqrt(55) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)7;
    /* Compute (n-1) mod 4 efficiently using bitwise AND */
    idx_t rem = (n - 1) & 3ULL;
    return (rem == 3) ? (seq_t)14 : (seq_t)2;
}
