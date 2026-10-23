/*
 * A047384  Numbers that are congruent to {0, 1, 5} mod 7.
 * https://oeis.org/A047384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * floor((n-1)/3) + f((n-1) % 3), where f(0)=0, f(1)=1,
 *            f(2)=5
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since the sequence is defined for n ≥ 1, guard against n=0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute the quotient and remainder of (n-1) divided by 3. */
    idx_t t = n - 1;           /* t ≥ 0 */
    idx_t q = t / 3;           /* floor((n-1)/3) */
    idx_t r = t % 3;           /* (n-1) mod 3 */

    /* Compute 7 * q in seq_t to avoid overflow for large n. */
    seq_t result = (seq_t)7 * (seq_t)q;

    /* Add the appropriate offset according to the remainder. */
    if (r == 1)
        result += 1;
    else if (r == 2)
        result += 5;  /* r==0 adds nothing */

    return result;
}
