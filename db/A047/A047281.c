/*
 * A047281  Numbers that are congruent to {0, 3, 6} mod 7.
 * https://oeis.org/A047281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*((n-1)/3) if n%3==1, * 7*((n+1)/3)-4 if n%3==2, *
 *            7*(n/3)-1 if n%3==0 (n>=1) - OEIS A047281
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based */
    idx_t rem = n % 3;
    if (rem == 1) {                 /* a(3k-2) = 7k-7 = 7*((n-1)/3) */
        return 7 * ((seq_t)((n - 1) / 3));
    } else if (rem == 2) {          /* a(3k-1) = 7k-4 = 7*((n+1)/3)-4 */
        return 7 * ((seq_t)((n + 1) / 3)) - (seq_t)4;
    } else {                        /* rem == 0 -> a(3k)   = 7k-1 */
        return 7 * ((seq_t)(n / 3)) - (seq_t)1;
    }
}
