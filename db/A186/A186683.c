/*
 * A186683  Total number of n-digit numbers requiring 18 positive biquadrates
 *          in their representation as sum of biquadrates.
 * https://oeis.org/A186683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 0 for n ≥ 5; a(1)=0, a(2)=2, a(3)=17, a(4)=5
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n)
    {
        case 1: return (seq_t)0;
        case 2: return (seq_t)2;
        case 3: return (seq_t)17;
        case 4: return (seq_t)5;
        default: return (seq_t)0;
    }
}
