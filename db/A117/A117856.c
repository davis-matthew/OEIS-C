/*
 * A117856  Number of palindromes of length n (in base 4).
 * https://oeis.org/A117856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*4^floor((n-1)/2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence alternative: a(1)=3, a(2)=3, a(n)=4*a(n-2) for n>=3 */

seq_t a(idx_t n)
{
    /* For n>=1 as per OEIS offset */
    idx_t k = (n - 1) / 2;   /* floor((n-1)/2) */
    seq_t v = 1;
    for (idx_t i = 0; i < k; ++i) {
        v *= 4;   /* multiply by 4 each step to get 4^k */
    }
    return 3 * v;
}
