/*
 * A080776  Oscillating sequence which rises to 2^(k-1) in k-th segment (k>=1)
 *          then falls back to 0.
 * https://oeis.org/A080776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0, * a(2n)=a(n)+a(n-1)+(n==1), * a(2n+1)=2*a(n)
 *            (OEIS A080776)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Forward declaration */
static inline seq_t a_rec(idx_t n);

/* External function requested by the build */
seq_t a(idx_t n)
{
    return a_rec(n);
}

/* Recursive evaluation of A080776 */
static inline seq_t a_rec(idx_t n)
{
    /* base cases */
    if (n == 0 || n == 1)
        return 0;

    /* odd: 2n+1 */
    if (n & 1U) {
        idx_t m = n >> 1;
        return 2 * a_rec(m);
    }

    /* even: 2n */
    idx_t m = n >> 1;
    seq_t left  = a_rec(m);
    seq_t right = a_rec(m - 1);
    seq_t res   = left + right;
    if (m == 1)               /* special case n==2 */
        res += 1;
    return res;
}
