/*
 * A042613  Denominators of continued fraction convergents to sqrt(835).
 * https://oeis.org/A042613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 68672711612*a(n-26) - a(n-52) for n>51 Initial terms
 *            a(0)=1, a(1)=1, a(2)=9, a(3)=10, a(4)=19, a(5)=29, a(6)=164,
 *            a(7)=193, … (continued CF denominators)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 25, u64 n <= 49, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t cache_len = 0;      /* number of terms currently stored */

/* Build the first 52 terms from the continued‑fraction convergents of sqrt(835). */
static void init_initial(void)
{
    const idx_t D = 835;                /* the square‑free part */
    const idx_t a0 = 28;                /* floor(sqrt(835)) */

    /* allocate space for the first 52 terms – indices 0 … 51 */
    cache = (seq_t *)malloc(52 * sizeof(seq_t));
    if (!cache) return;                 /* silently abort on allocation failure */

    /* Standard continued fraction setup for sqrt(D) */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    /* convergent numerators/denominators: p[-2]=0, p[-1]=1, q[-2]=1, q[-1]=0 */
    seq_t p_prev2 = (seq_t)0, p_prev1 = (seq_t)1;
    seq_t q_prev2 = (seq_t)1, q_prev1 = (seq_t)0;

    for (idx_t i = 0; i <= 51; ++i)
    {
        seq_t p = (seq_t)a * p_prev1 + p_prev2;
        seq_t q = (seq_t)a * q_prev1 + q_prev2;
        cache[i] = q;                  /* a(i) = q_i */

        /* shift for next step */
        p_prev2 = p_prev1;  p_prev1 = p;
        q_prev2 = q_prev1;  q_prev1 = q;

        /* next continued‑fraction term (skip after last needed) */
        if (i < 51)
        {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
        }
    }
    cache_len = 52;
}

/* Ensure that cache contains all terms up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len == 0)   /* first use: build initial terms */
        init_initial();

    if (n < cache_len) return;    /* already present */

    /* allocate more space (double the capacity each time) */
    size_t new_cap = cache_len ? cache_len * 2 : 64;
    while (new_cap <= n + 1) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) return;  /* silently abort on allocation failure */
    cache = new_cache;

    /* recurrence constant (fits in 64‑bit) */
    const seq_t COEF = (seq_t)68672711612ULL;

    /* compute missing terms using a(n) = 68672711612*a(n-26) - a(n-52) */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        cache[i] = COEF * cache[i - 26] - cache[i - 52];
    }
    cache_len = n + 1;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;   /* undefined for negative indices */
    ensure_cache(n);
    return cache[n];
}
