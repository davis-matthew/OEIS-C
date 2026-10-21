/*
 * A064544  Biquanimous numbers (or biquams): group the digits into two pieces
 *          (not necessarily equal or in order) with the same sum.
 * https://oeis.org/A064544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A064544 – Biquanimous numbers
 * The function enumerates integers, keeps those whose decimal digits can
 * be partitioned into two groups with the same sum, and returns the
 * nth term (0‑based).  No pre‑computed table is used, only a simple
 * subset‑sum check for each candidate.  Results are memoised in a
 * dynamically sized static array so that arbitrary‑order calls are
 * efficient.  The implementation uses only the C standard library
 * (malloc/realloc, memset, size_t, etc.) and works for any unsigned
 * seq_t and idx_t types provided by the build. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Constants that are large enough for the tested ranges.
   Adjust MAX_DIGITS if you need to handle numbers with more than
   200 decimal digits. */
#define MAX_DIGITS 200
#define MAX_TARGET (9 * MAX_DIGITS)

seq_t a(idx_t n)
{
    /* Persistent cache: cache[i] == a(i) for all i < cache_size. */
    static seq_t *cache = NULL;   /* pointer to array of seq_t */
    static idx_t cache_size = 0;  /* number of values stored */
    static idx_t cache_cap   = 0;  /* allocated capacity */

    /* Initialise the cache once. */
    if (cache_size == 0) {
        cache_cap   = 16;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* The first term of the sequence is 0. */
        cache[0]    = (seq_t)0;
        cache_size  = 1;
    }

    /* Return immediately if the requested index is already cached. */
    if (n < cache_size)
        return cache[n];

    /* Local buffers used for digit extraction and subset‑sum DP. */
    int  digits[MAX_DIGITS];
    bool dp[MAX_TARGET + 1];

    /* Start searching from the number following the last cached term. */
    seq_t candidate = cache[cache_size - 1];

    /* Generate terms until the nth term is found. */
    while (cache_size <= n) {
        /* Next integer.  We rely on the fact that the sequence terms
           fit into seq_t for the tested range. */
        candidate = candidate + 1;

        /* Extract decimal digits of candidate. */
        seq_t tmp = candidate;
        idx_t dlen = 0;
        int   sum  = 0;

        if (tmp == 0) { /* handle the number 0 explicitly */
            digits[0] = 0;
            dlen = 1;
        } else {
            while (tmp > 0 && dlen < MAX_DIGITS) {
                int d = (int)(tmp % 10);
                digits[dlen++] = d;
                sum += d;
                tmp = tmp / 10;
            }
            if (tmp > 0) /* number has more than MAX_DIGITS digits */
                continue;
        }

        /* The digit sum must be even. */
        if ((sum & 1) != 0)
            continue;

        int target = sum / 2;
        if (target > MAX_TARGET)
            continue; /* safety guard (should not happen) */

        /* Subset‑sum dynamic programming over the digits. */
        memset(dp, 0, sizeof(dp));
        dp[0] = true;
        for (idx_t i = 0; i < dlen; ++i) {
            int d = digits[i];
            for (int s = target; s >= d; --s) {
                if (dp[s - d])
                    dp[s] = true;
            }
        }

        if (dp[target]) { /* candidate qualifies as a biquanimous number */
            /* Resize cache if needed. */
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = candidate;
        }
    }

    return cache[n];
}
