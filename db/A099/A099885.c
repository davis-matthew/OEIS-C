/*
 * A099885  Central terms of the rows of the XOR difference triangle of the
 *          powers of 2 (A099884) so that a(n) = A099884(n, floor(n/2)).
 * https://oeis.org/A099885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2^⌊(n+1)/2⌋ * T(floor(n/2))
 * where T(k) = Σ_{j⊆k} 2^j  (sum taken over all submasks j of k).
 * TRT: The sum of powers of two over all submasks of k is the XOR binomial
 * transform of the powers of 2, i.e. A001317( k ).
 */

#include <stddef.h>
#include "../utils/pow2.h"

/* Cache for T(k) values */
static seq_t *t_cache = NULL;
static idx_t t_cache_len = 0;

/* Ensure that T(k) is computed for all k <= needed */
static void ensure_t_cache(idx_t needed)
{
    if (needed < t_cache_len) return;

    /* Grow cache */
    size_t new_len = needed + 1;
    t_cache = (seq_t *)realloc(t_cache, new_len * sizeof(seq_t));
    for (idx_t k = t_cache_len; k <= needed; ++k)
    {
        seq_t sum = 0;
        idx_t sub = k;
        /* Enumerate all submasks of k */
        do
        {
            sum += pow2(sub);
            if (sub == 0) break;
            sub = (sub - 1) & k;
        } while (1);
        t_cache[k] = sum;
    }
    t_cache_len = new_len;
}

/* External function required by the build */
seq_t a(idx_t n)
{
    idx_t k = n >> 1;                    /* floor(n/2) */
    idx_t exp = (n + 1) >> 1;            /* floor((n+1)/2) */

    /* Compute T(k) if not cached */
    ensure_t_cache(k);

    seq_t term = pow2(exp);
    return term * t_cache[k];
}
