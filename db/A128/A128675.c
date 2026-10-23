/*
 * A128675  Numbers m such that m^k does not divide the denominator of the
 *          m-th generalized harmonic number H(m,k) nor the denominator of the
 *          n-th alternating generalized harmonic number H'(m,k), for k = 5.
 * https://oeis.org/A128675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   a(n) = 37^k * mult[o] where k = (n-1)/4 + 1, o = (n-1) % 4, mult
 *            = {12, 18, 24, 36}
 * Range:     u8 none, u16 n <= 8, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


/* Multipliers for the four terms in each block. */
static const seq_t mult[4] = { (seq_t)12, (seq_t)18, (seq_t)24, (seq_t)36 };

/* Cache of successive powers of 37: pow_cache[i] = 37^i  (i≥0). */
static seq_t *pow_cache = NULL;   /* NULL until first allocation */
static idx_t  cache_size = 0;     /* highest exponent currently stored */

/*
 * Return 37^k, growing the cache if necessary.
 * When k == 0 the function returns 1 (by convention).
 */
static seq_t power_37(idx_t k)
{
    if (k == 0) return (seq_t)1;          /* 37^0 = 1 */

    /* Grow the cache so that it contains all powers up to k. */
    if (cache_size < k) {
        idx_t new_size = k + 1;            /* indices 0..k   */
        seq_t *new_cache = (seq_t*)realloc(pow_cache, sizeof(seq_t) * new_size);
        if (!new_cache) return (seq_t)0;   /* allocation failure */

        pow_cache = new_cache;

        /* Initialise exponent 0 if this is the first allocation. */
        if (cache_size == 0)
            pow_cache[0] = (seq_t)1;

        /* Compute the missing powers. */
        for (idx_t i = cache_size + 1; i <= k; ++i) {
            if (i == 1)
                pow_cache[i] = (seq_t)37;
            else
                pow_cache[i] = pow_cache[i - 1] * (seq_t)37;
        }
        cache_size = k;
    }
    return pow_cache[k];
}

/*
 * Sequence a(n) for n >= 1.  For n = 0 the function returns 0
 * (undefined, but avoids underflow when computing k).  The function
 * is thread‑safe in the sense that it only uses static local
 * state that is read‑only after initialization, and the allocation
 * performed by realloc() is performed only once per exponent range.
 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* Off‑by‑one guard */

    idx_t k = (n - 1) / 4 + 1;    /* exponent of 37   */
    idx_t o = (n - 1) % 4;        /* multiplier index */

    seq_t base = power_37(k);     /* 37^k   */
    if (base == (seq_t)0) return (seq_t)0;   /* allocation failure */

    return base * mult[o];
}
