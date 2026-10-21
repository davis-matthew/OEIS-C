/*
 * A206855  The sum of the degree of each root node over all rooted labeled
 *          trees on n nodes.
 * https://oeis.org/A206855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n^(n-1) - n^(n-2))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation: base^exp, exp is idx_t (unsigned 64‑bit). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)                /* a(0) = a(1) = 0 */
        return 0;

    /* n >= 2:
       a(n) = 2 * (n^(n-2)) * (n-1)  */
    seq_t pow = pow_seq((seq_t)n, n - 2);   /* n^(n-2) */
    seq_t res = pow * (seq_t)(n - 1);       /* n^(n-2)*(n-1) */
    res *= (seq_t)2;                        /* *2 */
    return res;
}
