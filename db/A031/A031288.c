/*
 * A031288  Position of n-th 1 in A007376.
 * https://oeis.org/A031288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: position of the n-th '1' in the infinite decimal concatenation of the
   positive integers (A007376).  Implementation performs an incremental search
   and memoises the discovered positions. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: convert a seq_t value into an array of decimal digits.
   The resulting array holds the most‑significant digit first.
   'buf' must have space for at least 128 digits; this is safe for
   all supported seq_t sizes (uint8_t … unsigned __int128). */
static size_t
digits_of_seq(seq_t v, uint8_t *buf, size_t bufmax)
{
    size_t dlen = 0;
    seq_t tmp = v;

    /* Handle zero explicitly (not needed for this sequence). */
    if (tmp == 0) {
        if (bufmax > 0) buf[0] = 0;
        return 1;
    }

    /* Extract digits least‑significant first. */
    while (tmp > 0) {
        if (dlen < bufmax)
            buf[dlen] = (uint8_t)(tmp % 10);
        tmp /= 10;
        ++dlen;
    }

    /* Reverse to most‑significant first. */
    for (size_t i = 0; i < dlen / 2; ++i) {
        uint8_t t = buf[i];
        buf[i] = buf[dlen - 1 - i];
        buf[dlen - 1 - i] = t;
    }
    return dlen;
}

seq_t a(idx_t n)
{
    /* Persistent state for memoisation. */
    static seq_t *pos_cache = NULL;   /* positions of '1' digits */
    static size_t   cache_cap = 0;
    static size_t   cache_len = 0;
    static seq_t   stream_pos = 0;     /* position in the decimal stream (1‑based) */
    static idx_t   last_num  = 0;      /* last integer processed */

    /* Initialize cache on first call. */
    if (!pos_cache) {
        cache_cap = 64;
        pos_cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!pos_cache) abort();  /* OOM: terminate */
    }

    /* Generate positions until we have at least n entries. */
    while (cache_len < (size_t)n) {
        /* Process next integer. */
        ++last_num;
        uint8_t digits[128];
        size_t dlen = digits_of_seq((seq_t)last_num, digits, sizeof(digits));

        /* Scan digits from most‑significant to least‑significant. */
        for (size_t i = 0; i < dlen; ++i) {
            ++stream_pos;                   /* move to next digit position */
            if (digits[i] == 1) {           /* found a '1' digit */
                /* Ensure capacity for the new entry. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(pos_cache, sizeof(seq_t) * new_cap);
                    if (!tmp) abort();      /* OOM */
                    pos_cache = tmp;
                    cache_cap = new_cap;
                }
                pos_cache[cache_len++] = stream_pos;
            }
        }
    }

    return pos_cache[n - 1];
}
