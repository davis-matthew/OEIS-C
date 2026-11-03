/*
 * A057701  Twin lucky numbers: (n,m) are lucky numbers with |n-m|=2.
 * https://oeis.org/A057701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1672, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9961 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Twin lucky numbers – numbers n that are lucky and part of a
   consecutive pair of lucky numbers differing by 2.
   a(n) is the n‑th such number (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Generate all lucky numbers up to 'limit' (inclusive).
   Return a malloc'd array and store the count in *outcnt. */
static uint32_t *
generate_lucky(uint32_t limit, size_t *outcnt)
{
    /* Create array containing all odd numbers up to limit. */
    size_t cap = (limit >> 1) + 10;                /* extra space */
    uint32_t *arr = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!arr) {
        *outcnt = 0;
        return NULL;
    }

    size_t cnt = 0;
    for (uint32_t i = 1; i <= limit; i += 2)
        arr[cnt++] = i;

    /* Classic lucky number sieve. */
    for (size_t i = 1; i < cnt; ++i) {
        uint32_t step = arr[i];
        if (step > cnt)
            break;                                /* no further deletions */

        size_t newcnt = 0;
        for (size_t j = 0; j < cnt; ++j) {
            if (((j + 1) % step) != 0)
                arr[newcnt++] = arr[j];
        }
        cnt = newcnt;
        if (cnt == 0)
            break;
    }

    *outcnt = cnt;
    return arr;
}

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
        seq_t *new_arr = mem_new(seq_t, lucky_cnt * 2);
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
            mem_free(twin_arr);
            twin_arr = new_arr;
            twin_cnt = len;
            return;
        }

        /* Not enough; free temporary array and try a larger sieve. */
        mem_free(new_arr);
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
