/*
 * A042535  Denominators of continued fraction convergents to sqrt(796).
 * https://oeis.org/A042535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Denominator q_k of the k‑th convergent of sqrt(796)
   Continued fraction:  a_{-1} = floor(sqrt(796)), m_{-1}=0, d_{-1}=1.
   For k≥0:
       m_k   = d_{k-1} * a_{k-1} – m_{k-1}
       d_k   = (796 – m_k^2) / d_{k-1}
       a_k   = (floor(sqrt(796)) + m_k) / d_k
       p_k   = a_k * p_{k-1} + p_{k-2}
       q_k   = a_k * q_{k-1} + q_{k-2}
   Initial values:  p_{-1}=1, p_0=floor(sqrt(796))=28,
                    q_{-1}=0, q_0=1.
   a(n) returns q_n.  */
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of denominators */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* Continued‑fraction state for the next term */
static idx_t  m = 0;             /* previous m, starts at m_{-1} */
static idx_t  d = 1;             /* previous d, starts at d_{-1} */
static idx_t  curr_a = 28;       /* a_{k-1}, starts at a_{-1} = floor(sqrt(796)) */

/* Numerators and denominators of the last two convergents */
static seq_t p_prev2 = 1;        /* p_{-1} */
static seq_t p_prev  = 28;       /* p_0  */
static seq_t q_prev2 = 0;        /* q_{-1} */
static seq_t q_prev  = 1;        /* q_0  */

/* Ensure that the cache contains at least n+1 terms. */
static void ensure_cache(idx_t n)
{
    while ((idx_t)cache_len <= n) {
        /* Expand cache if needed */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) { /* allocation failure – abort politely */
                /* In a real program we might handle this differently. */
                exit(1);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute the next continued‑fraction coefficients */
        idx_t m_next = d * curr_a - m;
        idx_t d_next = (796 - m_next * m_next) / d;
        idx_t a_next = (28 + m_next) / d_next;

        /* Compute the next convergent (p_k, q_k) */
        seq_t p_cur = (seq_t)a_next * p_prev + p_prev2;
        seq_t q_cur = (seq_t)a_next * q_prev + q_prev2;

        /* Store the new denominator */
        cache[cache_len++] = q_cur;

        /* Update state for the following iteration */
        p_prev2 = p_prev;  p_prev = p_cur;
        q_prev2 = q_prev;  q_prev = q_cur;
        m = m_next;        d = d_next;   curr_a = a_next;
    }
}

seq_t a(idx_t n)
{
    if (cache == NULL) {
        /* First call: initialise cache and continued‑fraction state */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache_len = 1;
        cache[0] = (seq_t)1;     /* q_0 = 1 */
    }
    ensure_cache(n);
    return cache[(size_t)n];
}
