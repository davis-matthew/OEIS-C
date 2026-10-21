/*
 * A205132  Least s(k) such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=2*j^2-j, the j-th hexagonal number.
 * https://oeis.org/A205132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the first hexagonal number whose residue modulo n
 *            repeats for some earlier index; this is the least s(k) such that
 *            n divides s(k)-s(j) for some j<k. Implementation uses a simple
 *            residue table
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* sequence defined only for n>=1 */

    size_t sz = (size_t)n;
    bool *seen = (bool *)calloc(sz, sizeof(bool));
    if (!seen) return 0;               /* allocation failed */

    seq_t result = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t kk = (seq_t)k;
        seq_t s  = 2 * kk * kk - kk;   /* hexagonal number s(k) */
        idx_t r  = (idx_t)(s % (seq_t)n);

        if (seen[r]) {                 /* residue already seen */
            result = s;
            break;
        }
        seen[r] = true;

        if (k > n) {                   /* safe fallback: repetition must occur by n+1 */
            result = s;
            break;
        }
    }

    free(seen);
    return result;
}
