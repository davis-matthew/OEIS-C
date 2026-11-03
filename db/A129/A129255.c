/*
 * A129255  Permutations with exactly 12 fixed points.
 * https://oeis.org/A129255
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n,12) * D(n-12) where D(k) is the number of
 *            derangements of k
 * Range:     u8 n <= 14, u16 n <= 16, u32 n <= 20, u64 n <= 27, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 194 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

static seq_t *der = NULL;
static idx_t der_len = 0;

seq_t a(idx_t n)
{
    if (n < 12)
        return (seq_t)0;           /* no permutations can have 12 fixed points */

    idx_t m = n - 12;              /* we need D(m) */

    /* initialise derangements cache with D[0] = 1 */
    if (der_len == 0) {
        der = mem_new(seq_t, 1);
        der[0] = (seq_t)1;
        der_len = 1;
    }

    /* extend cache up to D[m] if necessary */
    if (der_len <= m) {
        seq_t *tmp = mem_resize(seq_t, der, (m + 1));
        if (!tmp) abort();          /* out of memory */
        der = tmp;

        for (idx_t i = der_len; i <= m; ++i) {
            if (i == 1) {
                der[i] = (seq_t)0;  /* D[1] = 0 */
            } else {
                /* D[i] = (i-1) * (D[i-1] + D[i-2]) */
                der[i] = ((seq_t)(i - 1)) * (der[i - 1] + der[i - 2]);
            }
        }
        der_len = m + 1;
    }

    /* a(n) = binom(n,12) * D(n-12) */
    seq_t bin = binomial(n, 12);
    return bin * der[m];
}
