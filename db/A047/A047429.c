/*
 * A047429  Numbers that are congruent to {4, 5, 6} mod 8.
 * https://oeis.org/A047429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*ceil(n/3)-4 if n%3==1, 8*ceil((n-1)/3)-3 if n%3==2,
 *            8*(n/3)-2 if n%3==0
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 3) {
    case 1: {
        idx_t k = (n + 2) / 3;          // ceil(n/3)
        return (seq_t)k * (seq_t)8u - (seq_t)4u;
    }
    case 2: {
        idx_t k = (n + 1) / 3;
        return (seq_t)k * (seq_t)8u - (seq_t)3u;
    }
    default: {           // n % 3 == 0
        idx_t k = n / 3;
        return (seq_t)k * (seq_t)8u - (seq_t)2u;
    }
    }
}
