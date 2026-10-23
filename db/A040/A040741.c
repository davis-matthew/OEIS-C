/*
 * A040741  Continued fraction for sqrt(769).
 * https://oeis.org/A040741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = nth coefficient of the continued fraction expansion of
 *            sqrt(769). a(0) = 27, the integer part, and for n >= 1 the
 *            sequence repeats with period 20, ending with the term 2*a0 = 54
 * Range:     u8 n <= 0, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Number whose square‐root is expanded */
    const seq_t N = 769;
    /* Integer part of sqrt(N) */
    const seq_t a0 = 27;

    /* Static cache for the periodic part after the first term */
    static seq_t *period_terms = NULL;
    static size_t period_len   = 0;

    /* The zeroth term */
    if (n == 0)
        return a0;

    /* Compute the period only once */
    if (period_terms == NULL) {
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        size_t cap  = 0;
        size_t len  = 0;
        seq_t *arr  = NULL;

        while (1) {
            /* Next values according to the continued‑fraction algorithm */
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;

            /* Store the new coefficient */
            if (len >= cap) {
                cap = (cap == 0) ? 16 : cap * 2;
                arr = (seq_t *)realloc(arr, cap * sizeof(seq_t));
            }
            arr[len++] = a;

            /* Period ends when a == 2*a0 (for square‑free N) */
            if (a == 2 * a0)
                break;
        }

        period_terms = arr;
        period_len   = len;
    }

    /* Retrieve the requested term from the cached period */
    idx_t idx = (n - 1) % period_len;
    return period_terms[idx];
}
