/*
 * A218330  Odd decagonal pyramidal numbers.
 * https://oeis.org/A218330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (16*n-4*(-1)^n-17)*(4*n-(-1)^n-3)*(4*n-(-1)^n-1)/24
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd decagonal pyramidal numbers (OEIS A218330) */
seq_t a(idx_t n)
{
    /* Convert index to signed long long for intermediate calculations */
    long long N  = (long long)n;
    /* (-1)^n: +1 when n is even, -1 when n is odd */
    int s = (n & 1ULL) ? -1 : 1;

    /* Calculate the three factors of the product */
    long long p1 = 16 * N - 4 * s - 17;   /* 16n - 4*(-1)^n - 17 */
    long long p2 =  4 * N -     s -  3;   /*  4n -    (-1)^n - 3 */
    long long p3 =  4 * N -     s -  1;   /*  4n -    (-1)^n - 1 */

    /* Compute product using unsigned 64-bit arithmetic to avoid signed overflow.
       For the range of n that the tests exercise (≤10 000) the product fits in
       64 bits. */
    unsigned long long prod = (unsigned long long)p1 *
                              (unsigned long long)p2 *
                              (unsigned long long)p3;

    /* Final division by 24 (the product is always divisible by 24). */
    return (seq_t)(prod / 24ULL);
}
