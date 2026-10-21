/*
 * A069033  Powers of 8 with strictly increasing sum of digits.
 * https://oeis.org/A069033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th power of 8 in which the decimal digit sum
 *              is strictly larger than all digit sums of the earlier
 *              accepted powers.  a(1) = 1 (=8⁰).
 *
 * Implementation: iterate through successive powers of 8, keep the
 *                 last accepted digit sum, and cache the accepted
 *                 powers.  The helper function digit_sum computes the
 *                 sum of decimal digits of a value of type seq_t.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t digit_sum(seq_t x)
{
    seq_t s = (seq_t)0;
    while (x != (seq_t)0) {
        s += x % (seq_t)10;
        x /= (seq_t)10;
    }
    return s;
}

seq_t a(idx_t n)
{
    /* Static cache of accepted terms. */
    static seq_t *cache      = NULL;   /* array of powers of 8 */
    static size_t cache_cap  = 0;      /* allocated size of cache   */
    static size_t cache_len  = 0;      /* number of terms stored    */
    static seq_t current_pow  = (seq_t)1;   /* current 8^k                */
    static seq_t last_sum     = (seq_t)0;   /* digit sum of last accepted term */

    /* --- initialise on first call --- */
    if (cache_len == 0) {
        /* accept the first term 8⁰ = 1 */
        cache_cap = 8;                          /* initial size */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;

        cache[0]   = current_pow;
        last_sum   = digit_sum(current_pow);
        cache_len  = 1;        /* a(1) ready */
    }

    /* --- if requested term already cached, return it --- */
    if ((size_t)n <= cache_len)
        return cache[(size_t)(n - 1)];

    /* --- extend cache until it contains n terms --- */
    while (cache_len < (size_t)n) {
        /* compute next power of 8 (8ⁿ = 8^(n‑1) * 8) */
        seq_t next_pow = current_pow * (seq_t)8;
        seq_t cur_sum  = digit_sum(next_pow);

        if (cur_sum > last_sum) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_pow;
            last_sum = cur_sum;
        }

        current_pow = next_pow;
    }

    return cache[(size_t)(n - 1)];
}
