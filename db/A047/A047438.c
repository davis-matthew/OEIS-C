/*
 * A047438  Numbers that are congruent to {1, 5, 6} mod 8.
 * https://oeis.org/A047438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 8*k - 2 Formula: a(3k-1) = 8*k - 3 Formula: a(3k-2) =
 *            8*k - 7 (Works for all n >= 1, where k = ceil(n/3).)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  Below the offset behaviour
       is unspecified; returning zero is sufficient and safe. */
    if (n == 0)
        return (seq_t)0;

    switch (n % 3) {
        case 0: { /* n = 3k */
            idx_t k = n / 3;
            return (seq_t)8 * (seq_t)k - (seq_t)2;
        }
        case 1: { /* n = 3k + 1  =>  k' = (n+2)/3  so that n = 3k'-2 */
            idx_t k = (n + 2) / 3;
            return (seq_t)8 * (seq_t)k - (seq_t)7;
        }
        case 2: { /* n = 3k + 2  =>  k' = (n+1)/3  so that n = 3k'-1 */
            idx_t k = (n + 1) / 3;
            return (seq_t)8 * (seq_t)k - (seq_t)3;
        }
    }
    /* Unreachable, but keep compiler happy */
    return (seq_t)0;
}
