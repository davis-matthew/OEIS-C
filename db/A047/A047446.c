/*
 * A047446  Numbers that are congruent to {0, 1, 3, 5, 6} mod 8.
 * https://oeis.org/A047446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((8*n-7)/5) Equivalent piecewise form (no overflow
 *            for n>=1): let q = n/5, r = n%5 if r==0: a = 8*q - 2 if r==1: a
 *            = 8*q if r==2: a = 8*q + 1 if r==3: a = 8*q + 3 if r==4: a = 8*q
 *            + 5
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is assumed >= 1; behaviour for n==0 is undefined */
    idx_t q = n / 5;
    idx_t r = n % 5;

    switch (r)
    {
        case 0: return (seq_t)(8 * q - 2);
        case 1: return (seq_t)(8 * q);
        case 2: return (seq_t)(8 * q + 1);
        case 3: return (seq_t)(8 * q + 3);
        case 4: return (seq_t)(8 * q + 5);
        default: return 0; /* unreachable */
    }
}
