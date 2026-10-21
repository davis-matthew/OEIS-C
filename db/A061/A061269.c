/*
 * A061269  Squares with nonzero digits such that (1) each digit is a square
 *          and (2) the sum of the digits is a square.
 * https://oeis.org/A061269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061269: squares whose digits are 1, 4, or 9 and whose digit sum is a perfect square */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Function required by the test harness */
seq_t a(idx_t n)
{
    /* Offset 1: return 0 for indices < 1 (unused by the harness) */
    if (n < 1u) return (seq_t)0;

    /* ---- Static cache of computed terms ---- */
    static seq_t *cache = NULL;         /* array of found squares   */
    static size_t   cache_len = 0;      /* number of filled entries */
    static size_t   cache_cap = 0;      /* allocated capacity       */

    /* ---- Current integer to be squared ---- */
    static idx_t root = 1;

    /* ---- Pre‑computed table of small squares ---- */
    static bool is_sq[400] = { false };
    static bool table_done = false;
    if (!table_done) {
        for (int i = 0; i * i < 400; ++i)
            is_sq[i * i] = true;
        table_done = true;
    }

    /* Allocate initial cache space */
    if (cache == NULL) {
        cache_cap = 16;
        cache_len = 0;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* ---- Generate terms until we have at least n of them ---- */
    while (cache_len < n) {
        /* Compute the next square */
        seq_t sq = (seq_t)root * (seq_t)root;

        /* Check that all digits are 1, 4, or 9 and sum them */
        seq_t t   = sq;
        unsigned int sum = 0;
        bool ok = true;
        while (t > (seq_t)0) {
            /* Convert digit to unsigned int safely */
            unsigned int d = (unsigned int)(t % (seq_t)10);
            if (d == 0 || (d != 1 && d != 4 && d != 9)) {
                ok = false;
                break;
            }
            sum += d;
            t /= (seq_t)10;
        }

        /* Accept the square if the digit sum is a perfect square */
        if (ok && sum < 400 && is_sq[sum]) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = sq;
        }

        ++root;
    }

    return cache[n - 1];
}
