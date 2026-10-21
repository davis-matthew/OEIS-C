/*
 * A221372  A two-digit Look-and-Say sequence starting with 19: each term
 *          summarizes the increasing two-digit substrings of the previous
 *          term.
 * https://oeis.org/A221372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 19; for n > 0, a(n) is obtained from a(n‑1)
   by enumerating all two‑digit substrings of a(n‑1) in increasing
   order, counting their occurrences, and concatenating each count
   followed by the two‑digit number.  */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache       = NULL;
    static size_t  cache_cap  = 0;
    static idx_t   cache_used = 0;

    /* ----- grow cache as required ----- */
    if (cache_used <= n) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ----- compute missing terms ----- */
    for (idx_t i = cache_used; i <= n; ++i) {

        /* base term */
        if (i == 0) {
            cache[0] = 19;
            continue;
        }

        seq_t prev = cache[i - 1];

        /* ---- extract decimal digits (little‑endian) ---- */
        size_t digits_cap = 64;
        unsigned char *digits = (unsigned char *)malloc(digits_cap);
        size_t digits_len = 0;
        seq_t tmp = prev;

        while (tmp > 0) {
            if (digits_len == digits_cap) {
                digits_cap <<= 1;
                digits = (unsigned char *)realloc(digits, digits_cap);
            }
            digits[digits_len++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }

        /* ---- count two‑digit substrings ---- */
        uint64_t cnt[100] = {0};
        for (size_t j = 0; j + 1 < digits_len; ++j) {
            int val = digits[j + 1] * 10 + digits[j];
            cnt[val]++;
        }

        /* ---- build next term directly as seq_t ---- */
        seq_t val = 0;
        for (int valnum = 0; valnum < 100; ++valnum) {
            if (cnt[valnum] == 0) continue;

            /* convert count to decimal digits */
            unsigned long long c = cnt[valnum];
            char tmpbuf[32];
            int clen = 0;
            do {
                tmpbuf[clen++] = (char)('0' + (c % 10));
                c /= 10;
            } while (c > 0);

            /* append count (most significant first) */
            for (int k = clen - 1; k >= 0; --k)
                val = val * 10 + (tmpbuf[k] - '0');

            /* append the two‑digit number */
            val = val * 10 + (valnum / 10);
            val = val * 10 + (valnum % 10);
        }

        cache[i] = val;

        /* free temporary buffer */
        free(digits);
    }

    /* update number of stored terms */
    cache_used = (n + 1 > cache_used) ? (n + 1) : cache_used;

    return cache[n];
}
