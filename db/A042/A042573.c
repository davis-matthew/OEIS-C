/*
 * A042573  Denominators of continued fraction convergents to sqrt(815).
 * https://oeis.org/A042573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: Denominators of continued‑fraction convergents to sqrt(815)
    Recurrence:  q_{-2}=1, q_{-1}=0
                 q_n = a_n * q_{n-1} + q_{n-2}
    where a_0 = floor(sqrt(815)) = 28 and for n≥1
                 a_n = floor( (a_0 + m_n) / d_n )
    with m_{n+1} = d_n * a_n – m_n  and
          d_{n+1} = (815 – m_{n+1}^2) / d_n
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static const uint64_t D = 815; /* the radicand */
static const uint64_t A0 = 28; /* floor(sqrt(815)) */

/* Persistent cache of computed denominators */
static seq_t *cache = NULL;      /* array of q_n values */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t cache_len = 0;      /* number of terms stored */

/* State of the continued fraction generation for the next term */
static uint64_t m = 0;           /* current m (for the next a_n) */
static uint64_t d = 1;           /* current d (for the next a_n) */
static seq_t q_prev2 = 1;        /* q_{n-2} */
static seq_t q_prev1 = 0;        /* q_{n-1} */

seq_t a(idx_t n)
{
    /* Grow cache until it holds the n‑th term */
    while (cache_len <= n) {
        /* Ensure enough storage */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* out of memory */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        if (cache_len == 0) {
            /* Special handling for the 0‑th term (a_0) */
            seq_t a0 = (seq_t)A0;
            seq_t q0 = a0 * q_prev1 + q_prev2;   /* =1 */
            cache[cache_len++] = q0;

            /* Advance state for the next term (n = 1) */
            q_prev2 = q_prev1;     /* 0 */
            q_prev1 = q0;          /* 1 */

            uint64_t m_next = d * A0 - m;          /* m1 = 28 */
            uint64_t d_next = (D - m_next * m_next) / d; /* d1 = 31 */
            m = m_next;
            d = d_next;
        } else {
            /* Compute a_n for n >= 1 */
            seq_t a_n = ((seq_t)A0 + (seq_t)m) / (seq_t)d;

            seq_t q_n = a_n * q_prev1 + q_prev2;
            cache[cache_len++] = q_n;

            /* Shift for next iteration */
            q_prev2 = q_prev1;
            q_prev1 = q_n;

            /* Update continued fraction state */
            uint64_t m_next = d * (uint64_t)a_n - m;
            uint64_t d_next = (D - m_next * m_next) / d;
            m = m_next;
            d = d_next;
        }
    }
    return cache[n];
}
