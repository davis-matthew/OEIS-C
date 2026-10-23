/*
 * A132935  Concatenation of first n numbers of the lower Wythoff sequence.
 * https://oeis.org/A132935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat_{k=1}^{n+1} floor(k·φ) (lower Wythoff sequence)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    /* number of lower Wythoff terms to concatenate */
    idx_t m = n + 1;                     /* we need a(1) … a(m) */
    size_t limit = (size_t)(m * 3 / 2 + m) + 20;   /* upper bound for seen array */

    bool *seen = (bool *)calloc(limit, sizeof(bool));
    if (!seen) return 0;                /* @suppress missing error handling */

    seq_t result = 0;
    idx_t cur = 1;                       /* start with 1 – 0 is never in the sequence */

    for (idx_t k = 1; k <= m; ++k) {
        while ((size_t)cur < limit && seen[cur]) ++cur;   /* find mex */
        seq_t a_k = (seq_t)cur;                          /* floor(k·φ) */
        seen[cur] = true;                                /* mark used */

        idx_t upper = cur + k;                           /* companion upper Wythoff */
        if ((size_t)upper < limit) seen[upper] = true;

        seq_t pow = pow10((idx_t)digit_len(a_k));
        result = result * pow + a_k;

        ++cur;                                           /* next candidate integer */
    }

    free(seen);
    return result;
}
