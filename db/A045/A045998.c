/*
 * A045998  Binary Gleichniszahlen-Reihe (BGR) sequence: describe previous
 *          term (cf. A005150), reduce number of digits seen mod 2 (then for
 *          the purposes of this data-base, discard leading zeros).
 * https://oeis.org/A045998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 12, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* BGR sequence: a(0) = 1; for n > 0, a(n) is obtained by
   applying a binary look‑and‑say to a(n‑1) with the decimal digits
   of each run length reduced modulo 2, then discarding any leading 0s.
   The raw result before stripping is stored for use in the next step. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache_val = NULL;
    static char **cache_raw = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of terms computed so far */

    /* initialise cache and base case on first call */
    if (cache_val == NULL) {
        cache_cap = 1;
        cache_val = (seq_t *)malloc(sizeof(seq_t));
        cache_raw = (char **)malloc(sizeof(char *));
        if (!cache_val || !cache_raw) exit(EXIT_FAILURE);

        /* a(0) = 1 */
        cache_val[0] = (seq_t)1;
        char *raw0 = (char *)malloc(2);
        if (!raw0) exit(EXIT_FAILURE);
        raw0[0] = '1';
        raw0[1] = '\0';
        cache_raw[0] = raw0;
        cache_len = 1;
    }

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp_val = (seq_t *)realloc(cache_val, new_cap * sizeof(seq_t));
        char **tmp_raw = (char **)realloc(cache_raw, new_cap * sizeof(char *));
        if (!tmp_val || !tmp_raw) exit(EXIT_FAILURE);
        cache_val = tmp_val;
        cache_raw = tmp_raw;
        cache_cap = new_cap;
    }

    /* compute further terms up to n */
    while (cache_len <= (size_t)n) {
        /* previous raw representation */
        const char *prev_raw = cache_raw[cache_len - 1];
        size_t prev_len = strlen(prev_raw);

        /* estimate buffer size: each input digit may produce up to 7 output digits */
        size_t bufsize = prev_len * 8 + 1;
        char *tmp_buf = (char *)malloc(bufsize);
        if (!tmp_buf) exit(EXIT_FAILURE);

        size_t pos = 0;            /* current write position in tmp_buf */

        /* scan runs in prev_raw */
        for (size_t i = 0; i < prev_len; ) {
            char d = prev_raw[i];
            size_t run = 1;
            ++i;
            while (i < prev_len && prev_raw[i] == d) {
                ++run;
                ++i;
            }

            /* convert run length to decimal, store digits in reverse order */
            size_t rem = run;
            char digits[32];
            size_t dlen = 0;
            do {
                digits[dlen++] = '0' + (rem % 10);
                rem /= 10;
            } while (rem > 0);

            /* output reversed digits, reduced modulo 2, followed by the run digit */
            for (int k = (int)dlen - 1; k >= 0; --k) {
                char bit = (digits[k] - '0') & 1 ? '1' : '0';
                tmp_buf[pos++] = bit;
            }
            tmp_buf[pos++] = d;  /* the binary digit itself (0 or 1) */
        }

        /* create the new raw string (before stripping) */
        char *new_raw = (char *)malloc(pos + 1);
        if (!new_raw) exit(EXIT_FAILURE);
        memcpy(new_raw, tmp_buf, pos);
        new_raw[pos] = '\0';
        free(tmp_buf);

        /* compute numeric value of stripped representation */
        size_t start = 0;
        while (start < pos && new_raw[start] == '0') ++start;
        seq_t val = 0;
        for (size_t i = start; i < pos; ++i) {
            val = val * (seq_t)10 + (seq_t)(new_raw[i] - '0');
        }

        /* store in cache */
        cache_val[cache_len] = val;
        cache_raw[cache_len] = new_raw;
        ++cache_len;
    }

    return cache_val[n];
}
