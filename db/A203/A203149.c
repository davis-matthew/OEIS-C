/*
 * A203149  (n-1)-st elementary symmetric function of
 *          {2,8,26,80,242,...,-1+3^n}.
 * https://oeis.org/A203149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)-st elementary symmetric function of the numbers
 *            {-1+3^k : k = 1 .. n}. I.e. a(n) = Σ_{S ⊂ {1..n}, |S|=n-1}
 *            ∏_{k∈S} (3^k-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n=0, return 0 */
        return 0;

    /* e[k] holds the k‑th elementary symmetric sum after processing
       the first i elements.  Size n+1 is enough. */
    seq_t *e = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!e)          /* allocation failure – unlikely in tests */
        return 0;

    for (idx_t i = 0; i <= n; ++i)
        e[i] = 0;
    e[0] = 1;            /* e_0 = 1 */

    seq_t pow3 = (seq_t)3;        /* 3^i is needed for the i‑th element */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t x = pow3 - (seq_t)1;   /* current element 3^i - 1 */

        /* Update e in reverse so that e[k-1] is still the old value */
        for (idx_t k = i; k > 0; --k)
            e[k] = e[k] + x * e[k - 1];

        pow3 = pow3 * (seq_t)3;      /* advance 3^{i+1} */
    }

    seq_t res = e[n - 1];
    free(e);
    return res;
}
