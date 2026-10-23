/*
 * A047238  Numbers that are congruent to {0, 2} mod 6.
 * https://oeis.org/A047238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*((n-1)/2) + ((n%2==0)?2:0)
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1)=0, a(2)=2, a(3)=6, a(4)=8, ... */
    if (n == 0)   /* behavior below the offset is irrelevant */
        return 0;

    /* (n-1)/2 gives the number of complete 6‑steps completed */
    idx_t k = (n - 1) / 2;
    seq_t result = (seq_t)k * 6;

    /* if n is even, add the offset +2 for the 2‑mod‑6 term */
    if ((n & 1) == 0)
        result += 2;

    return result;
}
