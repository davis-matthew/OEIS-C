/*
 * A041353  Denominators of continued fraction convergents to sqrt(190).
 * https://oeis.org/A041353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let a(0)=1, a(1)=1, and for n≥2, a(n) is the denominator
   of the n‑th convergent to √190.  The convergents satisfy
   q_{-2}=1, q_{-1}=0, and   q_k = a_k*q_{k-1}+q_{k-2},
   where a_k are the continued‑fraction partial quotients of √190
   with a_0 = ⌊√190⌋ = 13.  */

#include <stdlib.h>
#include <stddef.h>

/* Cache of previously computed denominators. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;   /* number of terms already stored   */
static size_t cache_cap = 0;   /* space currently allocated       */

/* Continued fraction data for √190.  */
static const idx_t D       = 190u;  /* the radicand                */
static const idx_t SQRT_D  = 13u;   /* ⌊√190⌋, the first partial numerator   */
static idx_t   m = 0;              /* current m in the triple (m,d,a)       */
static idx_t   d = 1;              /* current d                               */
static idx_t   a_cur = SQRT_D;     /* current partial quotient a for term k  */

/* Denominator recurrence state:  q_{k-2} and q_{k-1}.  */
static seq_t q_prev2 = 1;  /* q_{-2} */
static seq_t q_prev1 = 0;  /* q_{-1} */

/* Compute cache[n] for all n up to the requested value.  */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)                /* already available */
        return;

    /* Grow the cache if necessary.  */
    if (cache_cap == 0)
        cache_cap = 16;
    while (cache_cap <= (size_t)n)
        cache_cap <<= 1;              /* multiply capacity by 2 */

    seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!new_cache)                   /* allocation failed */
        abort();
    cache = new_cache;

    /* Compute missing terms.  */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Current denominator from the recurrence. */
        seq_t a_mult = (seq_t)a_cur;
        seq_t q = a_mult * q_prev1 + q_prev2;
        cache[(size_t)i] = q;

        /* Shift the state for the next step. */
        seq_t old_q_prev2 = q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;

        /* Compute data for the next partial quotient.  */
        idx_t m_next = d * a_cur - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (SQRT_D + m_next) / d_next;

        m   = m_next;
        d   = d_next;
        a_cur = a_next;
    }

    cache_len = (size_t)n + 1;
}

/* Public function requested by the problem.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
