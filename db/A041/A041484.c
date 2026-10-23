/*
 * A041484  Numerators of continued fraction convergents to sqrt(259).
 * https://oeis.org/A041484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(-2)=0, a(-1)=1.
 * For n ≥ 0, a(n) = c(n) * a(n‑1) + a(n‑2)
 * where c(n) are the partial quotients of the continued‑fraction of √259.
 * Initial terms: a(0)=16, a(1)=161.  The coded method will memoise all
 * values so calls can come in any order. */

#include <stdlib.h>
#include <stdint.h>       /* for size_t */

static seq_t *cache = NULL;      /* memoised numerators */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t  cache_size = 0;    /* number of elements stored */

/* recurrence state */
static seq_t prev1 = 1;          /* a(-1) */
static seq_t prev2 = 0;          /* a(-2) */

/* continued‑fraction generator for √259 */
static long long m   = 0;
static long long d   = 1;
static long long cf  = 16;        /* a₀ */
static const long long D  = 259;  /* radicand */
static const long long A0 = 16;   /* floor(√259) */

seq_t a(idx_t n)
{
    /* Expand cache up to required index. */
    while (cache_size <= n) {
        /* Grow storage if needed. */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return 0;                /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute next numerator via recurrence. */
        seq_t cur = (seq_t)cf * prev1 + prev2;
        cache[cache_size] = cur;

        /* Generate next continued‑fraction coefficient. */
        long long m_next = d * cf - m;
        long long d_next = (D - m_next * m_next) / d;
        long long cf_next = (A0 + m_next) / d_next;

        /* Advance recurrence state. */
        prev2 = prev1;
        prev1 = cur;
        m = m_next;
        d = d_next;
        cf = cf_next;

        ++cache_size;
    }

    return cache[n];
}
