/*
 * A177179  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=9, k=1 and l=-1.
 * https://oeis.org/A177179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 9, a(n) = Σ_{p=0}^{n-1} a(p) * a(n-1-p) + (n-1)
 *            for n ≥ 2 (Recurrence derived from a(n+1)=∑_{p=0}^n
 *            (a(p)*a(n-p)+k)+l with k=1, l=-1) This is the OEIS sequence
 *            A177179
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/grow_seq.h"   /* provides void grow_seq(seq_t **, size_t *, size_t) */

seq_t a(idx_t n)
{
    /* Static cache: memo[a] holds a(0)..a[last] */
    static seq_t *memo      = NULL;
    static size_t    cap = 0;          /* allocated capacity */
    static idx_t    last_comp = (idx_t)-1;   /* last index computed */

    /* Ensure the array can hold up to a[n] */
    grow_seq(&memo, &cap, n + 1);

    /* Initialise base terms if not yet done */
    if (last_comp == (idx_t)-1) {
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)9;
        last_comp = 1;
    }

    /* Compute missing terms up to n */
    while (last_comp < n) {
        idx_t i = last_comp + 1;          /* index to compute */
        seq_t sum = 0;

        /* Convolution part: Σ_{p=0}^{i-1} a(p) * a(i-1-p) */
        idx_t half = (i - 1) / 2;
        for (idx_t p = 0; p <= half; ++p) {
            idx_t q = i - 1 - p;
            seq_t prod = memo[p] * memo[q];
            sum += prod;
            if (p != q) {       /* add the symmetrical counterpart */
                sum += prod;
            }
        }

        /* Add the linear term (i-1) */
        sum += (seq_t)(i - 1);
        memo[i] = sum;

        last_comp = i;
    }

    return memo[n];
}
