/*
 * A047496  Numbers that are congruent to {0, 2, 4, 5, 7} mod 8.
 * https://oeis.org/A047496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = let k = ceil(n/5). Then a(n) = 8*k - {1 if n%5==0, 8 if
 *            n%5==1, 6 if n%5==2, 4 if n%5==3, 3 if n%5==4}
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = n / 5;
    seq_t r = n % 5;
    seq_t k = q;
    if (r) k++;                 /* ceil(n/5) */
    seq_t base = k * ((seq_t)8);

    switch ((unsigned)r) {
        case 0: return base - 1;
        case 1: return base - 8;
        case 2: return base - 6;
        case 3: return base - 4;
        case 4: return base - 3;
    }
    return 0;   /* never reached */
}
