/*
 * A036918  a(n) = floor(e*(n-1)*(n-1)!).
 * https://oeis.org/A036918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(e*(n-1)*(n-1)!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*
 * Compute a(n) = floor(e*(n-1)*(n-1)!)
 * using the integer identity:
 *   a(n) = (n-1) * sum_{k=0}^{n-1} ((n-1)! / k!)
 * which avoids using floating point arithmetic.
 *
 * The calculation uses only integer additions, multiplications, and
 * divisions by small integers, all supported by the seq_t type.
 */
seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 0 by definition */
        return 0;

    idx_t m = n - 1;       /* m = n-1 */
    seq_t fact = 1;        /* m! */

    /* Compute m! */
    for (idx_t i = 2; i <= m; ++i)
        fact *= (seq_t)i;

    /* Compute S = sum_{k=0}^{m} m! / k!   using term_{k+1} = term_k / (k+1) */
    seq_t term = fact;     /* term_0 = m! / 0! = m! */
    seq_t sum  = term;

    for (idx_t k = 1; k <= m; ++k) {
        term /= (seq_t)k;  /* exact division: term_k = term_{k-1} / k */
        sum  += term;
    }

    /* a(n) = m * sum */
    return sum * (seq_t)m;
}
