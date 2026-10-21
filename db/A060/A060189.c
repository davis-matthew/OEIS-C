/*
 * A060189  A column and diagonal of A060187 (k=3).
 * https://oeis.org/A060189
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n-1) - n*3^(n-1) + n*(n-1)/2 (offset 3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation: base^exp for arbitrary seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0)
    {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;                     /* sequence is defined for n >= 3 */

    /* 5^(n-1) */
    seq_t pow5 = pow_seq_t((seq_t)5, n - 1);

    /* 3^(n-1) and n*3^(n-1) */
    seq_t pow3   = pow_seq_t((seq_t)3, n - 1);
    seq_t term2  = pow3 * (seq_t)n;          /* n * 3^(n-1) */

    /* n*(n-1)/2 */
    seq_t tri = ((seq_t)n * ((seq_t)n - 1)) / 2;

    /* 5^(n-1) + n*(n-1)/2 - n*3^(n-1) */
    return pow5 + tri - term2;
}
