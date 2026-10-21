/*
 * A066867  Numbers n such that 2^n has 7 as its fourth decimal digit from the
 *          right.
 * https://oeis.org/A066867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 6554, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A066867: a(n) are the exponents m for which 2^m has 7 as the
 * fourth decimal digit from the right (the thousands digit).
 *
 * The algorithm iterates exponents in increasing order, keeping 2^k mod 10000
 * to examine the thousands digit.  Whenever the digit is 7 the exponent is
 * stored in a static cache.  Subsequent calls reuse the cache, so each term
 * is generated only once.  This is O(n) in the index n and runs well under
 * the required time limits for n up to about 10000.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ----- static data for the cache ----- */
    static idx_t *cache      = NULL;   /* 1-based array of found exponents      */
    static size_t cache_size = 0;      /* number of elements currently stored  */
    static size_t cache_cap  = 0;      /* allocated capacity                    */
    static idx_t  last_exp   = 0;      /* exponent of the current 2^k          */
    static unsigned int pow_mod = 1;   /* 2^last_exp modulo 10000              */

    const unsigned int MOD = 10000u;

    /* ----- lazy initialization of the cache ----- */
    if (!cache) {
        cache_cap = 128;
        cache = (idx_t *)malloc(cache_cap * sizeof(idx_t));
        if (!cache) return 0;          /* fall back on allocation failure      */
    }

    /* ----- generate terms until we have at least n ----- */
    while (cache_size < n) {
        /* advance exponent and update power modulo 10000 */
        ++last_exp;
        pow_mod = (unsigned int)((unsigned long long)pow_mod * 2u % MOD);

        /* thousands digit of 2^last_exp is (pow_mod/1000)%10 */
        if (((pow_mod / 1000u) % 10u) == 7u) {
            /* enlarge cache if needed */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
                if (!tmp) return 0;          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_exp;
        }
    }

    return (seq_t)cache[n - 1];   /* n is 1‑based */
}
