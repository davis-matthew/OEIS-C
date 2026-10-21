/*
 * A057701  Twin lucky numbers: (n,m) are lucky numbers with |n-m|=2.
 * https://oeis.org/A057701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1672, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   generate_lucky
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Twin lucky numbers – numbers n that are lucky and part of a
   consecutive pair of lucky numbers differing by 2.
   a(n) is the n‑th such number (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/generate_lucky.h"

/* Static storage for computed twin lucky numbers. */
static seq_t  *twin_arr  = NULL;   /* array of twin lucky numbers */
static size_t  twin_cnt  = 0;       /* number of entries stored */

/* Ensure at least 'n' twin lucky numbers have been generated. */
static void
ensure_twin_count(idx_t n)
{
    if (twin_cnt >= (size_t)n)
        return;                     /* already ready */

    /* Estimate a sieve limit that is likely large enough. */
    size_t target = (size_t)n * 600;
    if (target < 10000)
        target = 10000;

    while (twin_cnt < (size_t)n) {
        /* Generate lucky numbers up to 'target'. */
        size_t lucky_cnt = 0;
        uint32_t *lucky = generate_lucky((uint32_t)target, &lucky_cnt);
        if (!lucky)
            return;                 /* memory exhaustion – abort */

        /* Collect twin lucky pairs: (x, x+2). */
        size_t len = 0;
        seq_t *new_arr = (seq_t *)malloc(lucky_cnt * 2 * sizeof(seq_t));
        if (!new_arr) {
            free(lucky);
            return;
        }

        for (size_t i = 0; i + 1 < lucky_cnt; ++i) {
            if (lucky[i + 1] - lucky[i] == 2u) {
                new_arr[len++] = (seq_t)lucky[i];
                new_arr[len++] = (seq_t)lucky[i + 1];
            }
        }

        free(lucky);

        if (len >= (size_t)n) {
            /* We now have enough terms. Replace the old cache. */
            free(twin_arr);
            twin_arr = new_arr;
            twin_cnt = len;
            return;
        }

        /* Not enough; free temporary array and try a larger sieve. */
        free(new_arr);
        target *= 2;                 /* double the sieve limit */
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;             /* undefined for n<1 */
    ensure_twin_count(n);
    return twin_arr[n - 1];
}
