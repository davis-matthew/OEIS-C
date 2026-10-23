/*
 * A010232  Continued fraction for sqrt(190).
 * https://oeis.org/A010232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   Continued‑fraction expansion of √190. a(0) = ⌊√190⌋ = 13. For k
 *            ≥ 0: m₀ = 0, d₀ = 1, a₀ = ⌊√190⌋, m_{k+1} = d_k * a_k – m_k,
 *            d_{k+1} = (190 – m_{k+1}²) / d_k, a_{k+1} = ⌊(a₀ + m_{k+1}) /
 *            d_{k+1}⌋. The sequence is periodic after the first term (period
 *            14)
 * Range:     u8 none, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Static storage for the initial term and one whole period. */
    static seq_t *seq = NULL;          /* array holding terms [0..period_len] */
    static size_t seq_len = 0;         /* number of stored terms */
    static size_t period_len = 0;      /* length of the repeating block */
    static int initialized = 0;        /* flag */

    if (!initialized) {
        /* Allocate initial space */
        size_t cap = 64;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return 0;           /* fallback on allocation failure */
        seq_len = 0;
        period_len = 0;

        /* Constants for √190 */
        const seq_t D = (seq_t)190ULL;
        const seq_t a0 = floor_sqrt((seq_t)190ULL);

        /* Store a(0) */
        seq[seq_len++] = a0;

        /* Initial recurrence values */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        /* Generate the period */
        while (1) {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;

            /* Grow buffer if necessary */
            if (seq_len == cap) {
                cap *= 2;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
                if (!seq) return 0;
            }
            seq[seq_len++] = a;        /* term a_{k+1} */

            /* Period ends when a == 2*a0 */
            if (a == (a0 + a0))
                break;
        }

        /* Now seq_len = period_len + 1 */
        period_len = seq_len - 1;
        initialized = 1;
    }

    if (n < seq_len)          /* n is within the precomputed block */
        return seq[n];

    /* Otherwise use the periodic property */
    /* Handle n == 0 separately to avoid unsigned overflow */
    idx_t idx = 1 + ((n - 1) % period_len);
    return seq[idx];
}
