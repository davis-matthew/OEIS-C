/*
 * A070199  Number of palindromes of length <= n.
 * https://oeis.org/A070199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*10^(n/2)-1 if n is even * a(n) = 11*10^((n-1)/2)-1 if n
 *            is odd * This is the number of palindromes of length ≤ n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset starts at 1, behaviour for n<1 is irrelevant */

    /* compute 10^k where k depends on parity of n */
    idx_t k;
    seq_t mult;

    if ((n & 1) == 0) {        /* even */
        k = n >> 1;            /* n/2 */
        mult = 2;              /* will multiply later by 2 */
    } else {                   /* odd */
        k = (n - 1) >> 1;      /* (n‑1)/2 */
        mult = 11;
    }

    seq_t p = 1;
    for (idx_t i = 0; i < k; ++i) {
        p *= (seq_t)10;
    }

    seq_t result = mult * p - 1;
    return result;
}
