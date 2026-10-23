/*
 * A047275  Numbers that are congruent to {0, 1, 6} mod 7.
 * https://oeis.org/A047275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { 7k-7, 7k-6, 7k-1 } for n=3k-2,3k-1,3k respectively,
 *            where k = (n+2)/3. This yields the OEIS sequence A047275
 *            (numbers congruent to 0,1,6 mod 7)
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; for n<1 the behaviour is undefined */
    seq_t k = ((seq_t)n + 2) / 3;   /* k = ceil(n/3) */
    switch (n % 3)
    {
        case 0:   /* n = 3k */
            return 7 * k - 1;
        case 1:   /* n = 3k-2 */
            return 7 * k - 7;
        case 2:   /* n = 3k-1 */
            return 7 * k - 6;
    }
    /* unreachable */
    return 0;
}
