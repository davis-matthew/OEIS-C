/*
 * A038685  Ninth powers ending nontrivially in a nonzero ninth power.
 * https://oeis.org/A038685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   “Ninth powers ending nontrivially in a nonzero ninth power”.
 *            A(n) is the nth integer of the form m⁹ such that – m ≢ 0 (mod
 *            10), – letting d = m mod 10, then d⁹ equals the last digits of
 *            m⁹ and the length of d⁹ is L, – m⁹ ≡ d⁹ (mod 10ᶫ), – m⁹ > 10⁹.
 *            The terms are listed in increasing order; a(1) =...
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function uses only operators allowed by the framework
   and is fully generic with respect to the compile-time type of
   `seq_t` and `idx_t`.  */

#include <stdlib.h>   /* for malloc, realloc, abort */

static const seq_t POW10[10] = {
    (seq_t)1,
    (seq_t)10,
    (seq_t)100,
    (seq_t)1000,
    (seq_t)10000,
    (seq_t)100000,
    (seq_t)1000000,
    (seq_t)10000000,
    (seq_t)100000000,
    (seq_t)1000000000
};

/* Static cache for generated terms.  */
static seq_t *cache = NULL;
static size_t cache_len = 0;  /* number of cached terms   */
static size_t cache_cap = 0;  /* allocated capacity        */

/* Next base to test for a ninth‑power candidate.  */
static idx_t next_m = 1;

/* Ensure `cache` has room for at least `needed` elements.  */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* allocation failure – signal error */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute number of decimal digits of a 32‑bit integer.  */
static int digit_count_uint32(uint32_t x)
{
    int d = 1;
    while (x >= 10) {
        x /= 10;
        ++d;
    }
    return d;
}

/* Return the nth term of the sequence.  */
seq_t a(idx_t n)
{
    /* Produce terms until the cache holds at least n elements.  */
    while (cache_len < n) {
        /* Skip bases ending in 0 – they cannot give a non‑zero ninth
           power at the end.  */
        if ((next_m % 10) != 0) {
            /* Compute m⁹.  */
            seq_t base = (seq_t)next_m;
            seq_t pow9 = base;
            for (int i = 1; i < 9; ++i)
                pow9 *= base;

            /* d = m mod 10, d⁹ and its length.  */
            int d = (int)(next_m % 10);
            uint32_t d_pow_uint = 1;
            for (int i = 0; i < 9; ++i)
                d_pow_uint *= (uint32_t)d;
            int len = digit_count_uint32(d_pow_uint);
            seq_t mod_base = POW10[len];

            /* Check the trailing ninth power condition.  */
            seq_t rem = pow9 % mod_base;
            if (rem == (seq_t)d_pow_uint && pow9 > (seq_t)1000000000) {
                /* Store succeeded term.  */
                grow_cache(cache_len + 1);
                cache[cache_len++] = pow9;
            }
        }
        ++next_m;
    }
    return cache[n - 1];
}
