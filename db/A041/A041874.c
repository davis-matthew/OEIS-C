/*
 * A041874  Numerators of continued fraction convergents to sqrt(459).
 * https://oeis.org/A041874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(459) */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined in the build environment. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms and state for incremental generation. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t len = 0;           /* number of terms stored (len >= 1 after init) */
    static seq_t p_prev = 0;         /* p_{k-2} */
    static seq_t p_curr = 0;         /* p_{k-1} */
    static seq_t m = 0;              /* current m in continued‑fraction algorithm */
    static seq_t d = 0;              /* current d in continued‑fraction algorithm */
    static seq_t a_val = 0;          /* current a in continued‑fraction algorithm */
    static bool initialized = false;

    /* Initialize on first call. */
    if (!initialized) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        /* a(0) = floor(sqrt(459)) = 21. */
        cache[0] = (seq_t)21;
        len = 1;
        /* State corresponding to this first term. */
        p_prev = (seq_t)1;          /* p_{-1} */
        p_curr = (seq_t)21;         /* p_0   */
        m = (seq_t)0;
        d = (seq_t)1;
        a_val = (seq_t)21;          /* a_0   */
        initialized = true;
    }

    /* Generate terms up to index n if necessary. */
    if ((size_t)n >= len) {
        /* Extend cache capacity as needed. */
        while (len <= (size_t)n) {
            if (len >= capacity) {
                capacity *= 2;
                cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            }

            /* Continued‑fraction recurrence: compute next m, d, a. */
            seq_t m_next = d * a_val - m;
            seq_t temp   = (seq_t)459 - m_next * m_next;   /* n = 459 */
            seq_t d_next = temp / d;
            seq_t a_next = ((seq_t)21 + m_next) / d_next;   /* a0 = 21 */

            /* Numerator recurrence: p_k = a_k * p_{k-1} + p_{k-2}. */
            seq_t p_next = a_next * p_curr + p_prev;

            /* Store the new term. */
            cache[len++] = p_next;

            /* Shift state for next iteration. */
            p_prev = p_curr;
            p_curr = p_next;
            m = m_next;
            d = d_next;
            a_val = a_next;
        }
    }

    return cache[(size_t)n];
}
