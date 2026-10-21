/*
 * A088556  Numbers of the form (4^n + 4^(n-1) + ... + 1) + (n mod 2).
 * https://oeis.org/A088556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^(n+1)-1)/3 + (n mod 2) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the first term; values for n < 1 are undefined,
       return 0 for safety. */
    if (n == 0)
        return 0;

    /* Compute 4^(n+1) by repeated multiplication. */
    seq_t pow4 = 1;

    /* Multiplying by 4 (n+1) times gives 4^(n+1). */
    for (idx_t i = 0; i <= n; ++i)
        pow4 *= (seq_t)4;

    /* Compute the geometric sum (4^(n+1)-1)/3.  The division is exact. */
    seq_t sum = (pow4 - (seq_t)1) / (seq_t)3;

    /* Add (n mod 2).  Using bitwise AND is equivalent to modulo 2. */
    return sum + (n & (idx_t)1);
}
