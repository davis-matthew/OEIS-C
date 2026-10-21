/*
 * A070307  Number of n X n matrices with nonnegative integer entries such
 *          that every row sum equals 3.
 * https://oeis.org/A070307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n+2,3)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle out-of-range offset if needed */
    if (n == 0) return 0;

    /* Compute C(n+2,3) = (n+2)(n+1)n / 6 safely using 128‑bit intermediate. */
    seq_t binom;
    {
        unsigned __int128 tmp = (unsigned __int128)(n + 2) * (n + 1) * n;
        binom = (seq_t)(tmp / 6);
    }

    /* Fast exponentiation: base^n */
    seq_t result = 1;
    seq_t base   = binom;
    idx_t exp    = n;
    while (exp) {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}
