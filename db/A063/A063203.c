/*
 * A063203  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          23 ).
 * https://oeis.org/A063203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-5) for n > 6 * initial values:
 *            a(1)=2, a(2)=5, a(3)=9, a(4)=13, a(5)=17, a(6)=19
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;                    /* out‑of‑range values never used */

    /* Allocate an array large enough for 1..n indices (t[0] unused) */
    seq_t *t = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!t) return 0;                       /* allocation failure → zero */

    /* Set the six initial terms */
    const seq_t init[6] = {2, 5, 9, 13, 17, 19};
    for (idx_t i = 1; i <= 6 && i <= n; ++i)
        t[i] = init[i - 1];

    if (n <= 6) {
        seq_t res = t[n];
        free(t);
        return res;
    }

    /* Compute subsequent terms using the recurrence */
    for (idx_t k = 7; k <= n; ++k)
        t[k] = t[k - 2] + t[k - 3] - t[k - 5];

    seq_t res = t[n];
    free(t);
    return res;
}
