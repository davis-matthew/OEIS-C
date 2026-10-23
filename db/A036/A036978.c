/*
 * A036978  Numbers for which the "describe what you see" transform A045918
 *          produces a prime.
 * https://oeis.org/A036978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 17, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_big, seq_from_str, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036978: Numbers for which the "describe what you see" transform
   (A045918) produces a prime.
   Computation: brute‑force enumeration of integers starting from 1,
   applying the transform and testing primality. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_from_str.h"
#include "../utils/seq_to_str.h"

/* Append the decimal representation of cnt to out, updating pos. */
static void append_count(int cnt, char *out, size_t *pos)
{
    char tmp[32];
    int tlen = 0;
    int v = cnt;
    if (v == 0) {
        tmp[tlen++] = '0';
    } else {
        while (v > 0) {
            tmp[tlen++] = '0' + (v % 10);
            v /= 10;
        }
        /* reverse */
        for (int i = 0, j = tlen - 1; i < j; ++i, --j) {
            char c = tmp[i];
            tmp[i] = tmp[j];
            tmp[j] = c;
        }
    }
    memcpy(out + *pos, tmp, tlen);
    *pos += tlen;
}

/* Apply the "describe‑what‑you‑see" transform to num and return the
   resulting value as seq_t. */
static seq_t describe_transform(seq_t num)
{
    /* Buffers large enough for all expected digits. */
    static char inbuf[4096];
    static char outbuf[8192];
    size_t inlen = sizeof(inbuf);
    seq_to_str(num, inbuf, &inlen);
    if (inlen >= sizeof(inbuf)) inlen = sizeof(inbuf) - 1;
    inbuf[inlen] = '\0';

    size_t outpos = 0;
    char current = inbuf[0];
    int count = 1;
    for (size_t i = 1; i < inlen; ++i) {
        char c = inbuf[i];
        if (c == current) {
            ++count;
        } else {
            append_count(count, outbuf, &outpos);
            outbuf[outpos++] = current;
            current = c;
            count = 1;
        }
    }
    /* final run */
    append_count(count, outbuf, &outpos);
    outbuf[outpos++] = current;
    outbuf[outpos] = '\0';

    return seq_from_str(outbuf);
}

/* Cache of found sequence values. */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;
static idx_t cache_cap   = 0;
static seq_t next_candidate = 1;

/* Return the n‑th term of A036978. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – a(1) is the first listed term */

    /* Fill the cache until we have n terms. */
    while (cache_len < n) {
        seq_t cand = next_candidate;
        ++next_candidate;

        seq_t transformed = describe_transform(cand);
        if (is_prime_big(transformed)) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
