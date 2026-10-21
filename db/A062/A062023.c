/*
 * A062023  a(n) = (n^(n+1) + n^(n-1))/2.
 * https://oeis.org/A062023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (n^(n+1) + n^(n-1))/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 1 with exact arithmetic                     */
/* The function uses exponentiation by squaring on seq_t (arbitrary    */
/* precision if needed).                                             */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        if (exp > 1) base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t b = (seq_t)n;
    seq_t pow1 = pow_seq(b, n + 1);   /* n^(n+1) */
    seq_t pow2 = pow_seq(b, n - 1);   /* n^(n-1) */
    seq_t sum  = pow1 + pow2;
    return sum / 2;
}
