/*
 * A193508  a(n) = n if n is not a power of 2 and a(2^n) = a(n).
 * https://oeis.org/A193508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is not a power of 2; a(2^k) = a(k) for k ≥ 0
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 0 by definition */
        return (seq_t)0;

    /* Check if n is a power of 2: n & (n-1) == 0 */
    if ((n & (n - 1)) == (idx_t)0) {
        /* Determine exponent k such that n = 2^k */
        idx_t k = 0;
        while (((idx_t)1 << k) < n)
            k++;

        /* Recursive definition: a(2^k) = a(k) */
        return a(k);
    }

    /* For all other n, a(n) = n */
    return (seq_t)n;
}
