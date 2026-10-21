/*
 * A225756  Runs of consecutive numbers with the same number of divisors.
 * https://oeis.org/A225756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A225756: runs of consecutive numbers with the same number of divisors.
   The sequence lists every integer that belongs to a consecutive run of
   length at least two where all members have equal divisor count.
   The function computes the nth term (1‑based) by generating numbers
   until it has produced at least n terms, storing the results in a static
   cache.  This is efficient enough for n up to about 10 000. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Count divisors of a 64‑bit integer by trial division.  The numbers
   examined here are small enough that this is fast. */
static int divisor_count(idx_t n)
{
    int cnt = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 1;
            temp /= p;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }
            cnt *= (exp + 1);
        }
    }
    if (temp > 1)
        cnt *= 2;
    return cnt;
}

/* Static cache buffer large enough for the typical usage (≈10 000 terms). */
#define MAX_CACHE 200000
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;

/* State maintained between calls. */
static idx_t last_checked = 0;
static int prev_div = 0;
static idx_t run_start = 0;
static int run_len = 0;

/* Main access function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined, but harmless */

    /* Generate terms until we have at least n cached. */
    while (cache_len < n) {
        ++last_checked;
        int cur_div = divisor_count(last_checked);

        if (last_checked == 1) {      /* initial value */
            run_start = 1;
            run_len = 1;
            prev_div = cur_div;
            continue;
        }

        if (cur_div == prev_div) {    /* continuing a run */
            if (run_len == 1) {        /* second element – output start */
                if (cache_len < MAX_CACHE)
                    cache[cache_len++] = run_start;
            }
            if (cache_len < MAX_CACHE)
                cache[cache_len++] = (seq_t)last_checked;
            ++run_len;
        } else {                       /* run broken – start new run */
            run_start = last_checked;
            run_len = 1;
        }

        prev_div = cur_div;

        /* Safety check to avoid exceeding the static array. */
        if (cache_len >= MAX_CACHE)
            break;
    }

    return cache[n - 1];
}
