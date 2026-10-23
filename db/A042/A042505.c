/*
 * A042505  Denominators of continued fraction convergents to sqrt(780).
 * https://oeis.org/A042505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(780).
   a(0) = 1, a(1) = 1.
   Recurrence: let q_{-1} = 0, q_0 = 1.
   For n ≥ 1 set a_n equal to the continued‑fraction partial
   quotients of √780: a_0 = 27, and for n ≥ 1
   a_n repeats [1, 12, 1, 54].
   Then q_n = a_n · q_{n‑1} + q_{n‑2}.
   This function memoises the values so that each term is
   computed only once, in O(n) time overall. */

#include <stdlib.h>

static seq_t *q_cache = NULL;          /* memoised denominator array   */
static idx_t q_cache_size = 0;         /* number of computed elements   */
static idx_t q_cache_capacity = 0;     /* current storage capacity      */

/* periodic part of the continued fraction after the first term */
static const seq_t period[4] = {
    (seq_t)1, (seq_t)12, (seq_t)1, (seq_t)54
};

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (needed <= (size_t)q_cache_capacity)
        return;
    idx_t new_cap = q_cache_capacity ? q_cache_capacity : 4;
    while (new_cap < (idx_t)needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(q_cache, sizeof(seq_t) * new_cap);
    if (!tmp)
        abort();   /* out of memory – abort */
    q_cache = tmp;
    q_cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise base values once */
    if (q_cache_size == 0) {
        ensure_cache(2);           /* at least indices 0 and 1          */
        q_cache[0] = (seq_t)1;     /* q₀                                */
        q_cache[1] = (seq_t)1;     /* a₁ = 1 ⇒ q₁ = 1                   */
        q_cache_size = 2;
    }

    /* if the requested index is already known, return it */
    if (n < q_cache_size)
        return q_cache[(size_t)n];

    /* grow caching array if necessary */
    ensure_cache((size_t)n + 1);

    /* compute missing terms using the recurrence */
    for (idx_t i = q_cache_size; i <= n; ++i) {
        seq_t a_i = period[(i - 1) % 4];                 /* a_i for i ≥ 1 */
        q_cache[(size_t)i] = a_i * q_cache[(size_t)i - 1]
                            + q_cache[(size_t)i - 2];
    }
    q_cache_size = n + 1;
    return q_cache[(size_t)n];
}
