/*
 * A047321  Numbers that are congruent to {1, 5, 6} mod 7.
 * https://oeis.org/A047321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*ceil(n/3)-6 if n%3==1, * a(n) = 7*ceil(n/3)-2 if
 *            n%3==2, * a(n) = 7*ceil(n/3)-1 if n%3==0 * (Equivalently:
 *            a(3k-2)=7k-6, a(3k-1)=7k-2, a(3k)=7k-1)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, behaviour for 0 is irrelevant */
    seq_t k = ((seq_t)n + 2) / 3;     /* ceil(n/3) */
    idx_t r = n % 3;
    if (r == 1) return 7 * k - 6;
    if (r == 2) return 7 * k - 2;
    return 7 * k - 1;                /* r == 0 */
}
