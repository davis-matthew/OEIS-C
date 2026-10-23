/*
 * A041954  Numerators of continued fraction convergents to sqrt(500).
 * https://oeis.org/A041954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 1860498*a(n-14) - a(n-28) Initial terms a(0) … a(27) are
 *            the numerators of the continued‑fraction convergents to
 *            sqrt(500)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure that the cache can hold at least `need` elements.  */
static void ensure_cache_size(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* Allocation failed. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Fills the first 28 terms from the continued fraction of sqrt(500). */
static void init_first_terms(void)
{
    /* Continued–fraction setup for sqrt(500). */
    const unsigned long long D   = 500ULL;
    const unsigned long long a0  = 22ULL;          /* floor(sqrt(500)) */
    unsigned long long m   = 0;
    unsigned long long d   = 1;
    unsigned long long a_q = a0;

    seq_t p_prev2 = 0;   /* p(-2) */
    seq_t p_prev1 = 1;   /* p(-1) */
    seq_t p;

    for (idx_t i = 0; i < 28; ++i) {
        p = (seq_t)a_q * p_prev1 + p_prev2;
        cache[i] = p;

        /* Next partial quotient.  */
        m = d * a_q - m;
        d = (D - m * m) / d;
        a_q = (a0 + m) / d;

        /* Shift previous convergents.  */
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    cache_len = 28;
}

/* Sequence a(n).  */
seq_t a(idx_t n)
{
    /* Pre‑compute the first 28 terms lazily.  */
    if (cache_len == 0) {
        ensure_cache_size(28);
        init_first_terms();
    }

    if (n < cache_len) {
        return cache[n];
    }

    /* Extend cache up to n using the recurrence.  */
    idx_t old_len = cache_len;
    idx_t new_len = n + 1;
    ensure_cache_size(new_len);

    /* a(n) = 1860498*a(n-14) - a(n-28)  */
    const seq_t coef = (seq_t)1860498ULL;

    for (idx_t i = old_len; i <= n; ++i) {
        cache[i] = coef * cache[i - 14] - cache[i - 28];
    }
    cache_len = new_len;
    return cache[n];
}
