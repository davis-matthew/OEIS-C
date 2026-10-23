/*
 * A042913  Denominators of continued fraction convergents to sqrt(988).
 * https://oeis.org/A042913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 29098901054*a(n-16) - a(n-32) for n>31; initial values
 *            a(0) .. a(31) are the first 32 denominators of the
 *            continued‑fraction convergents to sqrt(988)
 * Range:     u8 none, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void compute_initial_terms(seq_t *arr)
{
    /* floor(sqrt(988)) = 31 */
    const seq_t a0 = (seq_t)31;
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a = a0;

    seq_t p_prev = (seq_t)1;
    seq_t p_cur  = a0;
    seq_t q_prev = (seq_t)0;
    seq_t q_cur  = (seq_t)1;

    arr[0] = q_cur;                     /* q0 = 1 */
    for (idx_t i = 1; i < 32; ++i) {
        m = d * a - m;
        d = ((seq_t)988 - m * m) / d;
        a = (a0 + m) / d;

        seq_t q_next = a * q_cur + q_prev;
        seq_t p_next = a * p_cur + p_prev;

        q_prev = q_cur;  q_cur = q_next;
        p_prev = p_cur;  p_cur = p_next;

        arr[i] = q_cur;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_len = 0;

    if (n < cached_len)
        return cache[n];

    if (cached_len == 0) {
        /* allocate space for the first 32 terms */
        cache = (seq_t*)calloc(32, sizeof(seq_t));
        if (!cache)
            abort();
        compute_initial_terms(cache);
        cached_len = 32;
    }

    if (n < cached_len)
        return cache[n];

    /* extend the cache if needed */
    size_t new_len = (size_t)n + 1;
    cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!cache)
        abort();

    /* compute terms using the recurrence */
    for (size_t i = cached_len; i <= n; ++i) {
        cache[i] = ((seq_t)29098901054ULL) * cache[i-16] - cache[i-32];
    }
    cached_len = new_len;

    return cache[n];
}
