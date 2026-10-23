/*
 * A047613  Numbers that are congruent to {1, 2, 4, 5} mod 8.
 * https://oeis.org/A047613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/4) + [1,2,4,5][(n-1) mod 4]
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (Numbers congruent to {1,2,4,5} mod 8) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1; n>=1 is expected */
    idx_t m = n - 1;
    idx_t g = m / 4;                   /* floor((n-1)/4)      */
    idx_t r = m % 4;                   /* (n-1) mod 4        */
    static const seq_t offs[4] = {1, 2, 4, 5};
    seq_t base = (seq_t)g * (seq_t)8;   /* 8 * floor((n-1)/4) */
    return base + offs[r];
}
