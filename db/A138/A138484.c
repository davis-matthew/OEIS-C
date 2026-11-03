/*
 * A138484  Say what you see in previous term, from the right, reporting total
 *          number for each digit encountered. Initial term is 0.
 * https://oeis.org/A138484
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

/*
 * Recurrence: a(0) = 0;
 *             For n > 0, a(n) is obtained from a(n-1) by
 *             reading the digits of a(n-1) from right to left,
 *             and for each distinct digit d encountered,
 *             appending the decimal representation of the
 *             total count of d in a(n-1) followed by d.
 *
 * This implements the OEIS sequence A138484.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute the next term from the previous one. */
static seq_t term_from_prev(const seq_t prev)
{
    unsigned int cnt[10] = {0};

    /* Count all digits in prev. */
    seq_t tmp = prev;
    if (tmp == 0) {
        cnt[0] = 1;
    } else {
        while (tmp > 0) {
            unsigned int d = (unsigned int)(tmp % 10);
            cnt[d]++;
            tmp /= 10;
        }
    }

    /* Scan from right to left, build output digits. */
    bool seen[10] = {false};
    unsigned char outd[1024];   /* sufficient for typical term lengths */
    size_t outlen = 0;

    if (prev == 0) {                     /* special case 0 */
        outd[outlen++] = 1;               /* count of 0 is 1 */
        outd[outlen++] = 0;
    } else {
        tmp = prev;
        while (tmp > 0) {
            unsigned int d = (unsigned int)(tmp % 10);
            if (!seen[d]) {
                seen[d] = true;
                unsigned int c = cnt[d];

                /* append decimal digits of count c */
                if (c < 10) {
                    outd[outlen++] = (unsigned char)c;
                } else {
                    unsigned char stack[20];
                    size_t st = 0;
                    unsigned int t = c;
                    while (t > 0) {
                        stack[st++] = (unsigned char)(t % 10);
                        t /= 10;
                    }
                    for (size_t k = st; k > 0; --k)
                        outd[outlen++] = stack[k - 1];
                }
                outd[outlen++] = (unsigned char)d;
            }
            tmp /= 10;
        }
    }

    /* Convert the collected digits back to a seq_t value. */
    seq_t res = 0;
    for (size_t i = 0; i < outlen; ++i)
        res = res * 10 + outd[i];
    return res;
}

/* Cache for computed values. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure the cache contains all terms up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = mem_resize(seq_t, cache, new_len);
    if (!new_cache)
        exit(1);                     /* out of memory */

    cache = new_cache;

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[0] = 0;
        else
            cache[i] = term_from_prev(cache[i - 1]);
    }
    cache_len = new_len;
}

/* External linkage function required by the test harness. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
