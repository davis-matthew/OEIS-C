/*
 * A022856  a(n) = n-2 + Sum_{i = 1..n-2} (a(i+1) mod a(i)) for n >= 3 with
 *          a(1) = a(2) = 1.
 * https://oeis.org/A022856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = a(2) = a(3) = 1; for n > 3, a(n) = (n-4)*(n-3)/2 + 2
 *            (equivalent to a(n) = (n^2 - 7*n + 16)/2)
 * Range:     u8 n <= 26, u16 n <= 365, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)1;
    /* For n >= 4 set a = n-4, b = n-3.
       a*b is always even (adjacent numbers), so the division is exact. */
    seq_t a = (seq_t)(n - 4);
    seq_t b = (seq_t)(n - 3);
    return (a * b) / 2 + (seq_t)2;
}
