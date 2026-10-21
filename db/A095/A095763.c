/*
 * A095763  Numbers whose name in English contains an "i".
 * https://oeis.org/A095763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) is the nth integer whose English name contains an 'i'. The
 *            function builds a list of such numbers on demand, checking each
 *            integer's English representation. For numbers ≥ 1 000 000 the
 *            word "million" already contains an 'i', so they are accepted
 *            immediately
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   has_i_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this code must compile as C11 or C++17.  It uses only
   <stddef.h>, <stdint.h> and <stdlib.h>.  The pointer returned by
   realloc is cast to seq_t*, which is safe for both plain C types
   and C++ classes. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_i_part.h"

/* English word tables for 0–19 and the tens. */
static const char *const units_str[20] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

static const char *const tens_str[8] = {
    "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};

/* Return 1 if the English name of n contains an 'i'. */
static int has_i_uint64(uint64_t n)
{
    if (n == 0) return 0;                     /* zero is never counted */

    if (n < 20) return has_i_part(units_str[n]);

    if (n < 100) {
        uint64_t t = n / 10;
        if (has_i_part(tens_str[t - 2])) return 1;
        uint64_t u = n % 10;
        if (u && has_i_part(units_str[u])) return 1;
        return 0;
    }

    if (n < 1000) {
        uint64_t h = n / 100;
        if (has_i_part(units_str[h])) return 1;
        uint64_t r = n % 100;
        if (r && has_i_uint64(r)) return 1;
        return 0;
    }

    if (n < 1000000) {                      /* thousands */
        uint64_t k = n / 1000;
        if (has_i_uint64(k)) return 1;        /* "thousand" has no i */
        uint64_t r = n % 1000;
        if (r && has_i_uint64(r)) return 1;
        return 0;
    }

    /* ≥ 1 000 000: "million" contains 'i' */
    return 1;
}

/* Main sequence function.  Caches results to allow arbitrary call order. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of computed terms   */
    static idx_t cache_cap = 0;          /* allocated capacity        */
    static idx_t cache_len = 0;          /* number of valid terms     */
    static uint64_t last_checked = 0;    /* largest number already tested */

    /* Grow the cache if needed */
    if (cache_len < n) {
        idx_t needed = n;
        if (cache_cap == 0) cache_cap = 64;
        while (cache_cap < needed) cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) exit(1);              /* allocation failure */
    }

    /* Generate missing terms */
    uint64_t candidate = last_checked + 1;
    while (cache_len < n) {
        if (has_i_uint64(candidate)) {
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    last_checked = candidate - 1;

    return cache[n - 1];
}
