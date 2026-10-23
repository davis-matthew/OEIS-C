/*
 * A042724  Numerators of continued fraction convergents to sqrt(892).
 * https://oeis.org/A042724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=29, a(1)=30, for n≥2: a(n)=c[n-1]*a(n-1)+a(n-2), where the
 *            partial–quotient cycle for sqrt(892) is c = {1,6,2,14,2,6,1,58}
 *            repeating
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const uint8_t cf_coeff[8] = {1, 6, 2, 14, 2, 6, 1, 58};

static seq_t *memo = NULL;
static size_t memo_len = 0;

/* Compute the n‑th term of A042724 */
seq_t a(idx_t n)
{
    /* Ensure the cache is initialized */
    if (memo == NULL) {
        memo_len = 1;
        memo = (seq_t *)malloc(sizeof(seq_t));
        memo[0] = (seq_t)29;              /* a(0) */
    }

    /* Grow the cache up to index n (if required) */
    if (memo_len <= (size_t)n) {
        size_t needed = (size_t)n + 1;
        memo = (seq_t *)realloc(memo, sizeof(seq_t) * needed);

        for (size_t i = memo_len; i <= n; ++i) {
            if (i == 1) {
                /* a(1) = 1*a(0) + 1 */
                memo[1] = (seq_t)cf_coeff[0] * memo[0] + (seq_t)1;
            } else {
                seq_t coeff = (seq_t)cf_coeff[(i - 1) % 8];
                memo[i] = coeff * memo[i - 1] + memo[i - 2];
            }
        }
        memo_len = needed;
    }

    return memo[(size_t)n];
}
