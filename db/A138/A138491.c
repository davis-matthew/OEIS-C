/*
 * A138491  Say what you see in previous term, from the right, reporting total
 *          number for each digit encountered. Initial term is 7.
 * https://oeis.org/A138491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 7
   a(n) = NEXT(a(n-1))
   where NEXT(x) is obtained by scanning x from right to left
   and, for each digit first encountered, appending the total count
   of that digit in x followed by the digit itself. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Helper: produce next term from a previous term */
static seq_t get_next(seq_t prev)
{
    unsigned char total[10] = {0};
    seq_t tmp = prev;

    /* count all digits */
    if (tmp == 0) {          /* not needed for this sequence but safe */
        total[0] = 1;
    }
    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 10);
        total[d] += 1;
        tmp /= 10;
    }

    /* produce the description */
    unsigned char seen[10] = {0};
    char buf[512];           /* sufficient for all terms */
    size_t pos = 0;
    tmp = prev;

    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 10);
        if (!seen[d]) {
            seen[d] = 1;

            /* convert count to decimal string */
            unsigned cnt = total[d];
            char cntbuf[32];
            int clen = 0;
            unsigned val = cnt;
            if (val == 0) {
                cntbuf[clen++] = '0';
            } else {
                while (val > 0) {
                    cntbuf[clen++] = '0' + (val % 10);
                    val /= 10;
                }
                /* reverse */
                for (int i = 0; i < clen / 2; ++i) {
                    char c = cntbuf[i];
                    cntbuf[i] = cntbuf[clen - 1 - i];
                    cntbuf[clen - 1 - i] = c;
                }
            }
            for (int i = 0; i < clen; ++i) {
                buf[pos++] = cntbuf[i];
            }
            buf[pos++] = (char)('0' + d);
        }
        tmp /= 10;
    }

    /* convert buffer of digits back to seq_t */
    seq_t result = 0;
    for (size_t i = 0; i < pos; ++i) {
        result = result * 10 + (seq_t)(buf[i] - '0');
    }
    return result;
}

/* Cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

/* Public sequence function */
seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) return (seq_t)0;   /* fallback */
        cache[0] = 7;        /* a(0) */
        cache_len = 1;
    }

    while (cache_len <= n) {
        seq_t next = get_next(cache[cache_len - 1]);
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_len] = next;
        ++cache_len;
    }
    return cache[n];
}
