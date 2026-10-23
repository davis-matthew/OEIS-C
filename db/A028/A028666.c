/*
 * A028666  a(n) = order of the orthogonal group O_n(2) if n is odd or
 *          O^(+)_n(2) if n is even.
 * https://oeis.org/A028666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n * ∏_{k=0}^{n-1} (4^n - 4^k) (OEIS A028666)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 4^n */
    seq_t pow4n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow4n *= 4;            /* pow4n = 4^n */

    /* ∏_{k=0}^{n-1} (4^n - 4^k) */
    seq_t result = 1;
    seq_t pow4k = 1;           /* 4^k */
    for (idx_t k = 0; k < n; ++k) {
        result *= (pow4n - pow4k);
        pow4k *= 4;            /* next 4^k */
    }

    /* Multiply by the final 4^n factor */
    result *= pow4n;
    return result;
}
