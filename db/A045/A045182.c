/*
 * A045182  Numbers whose base-5 representation contains exactly two 0's and
 *          two 2's.
 * https://oeis.org/A045182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑5 representation contains exactly two 0's and two 2's.
   The function enumerates natural numbers and checks the base‑5 digit counts until the
   n‑th such number is found.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache of already computed terms */
static seq_t *cache     = NULL;
static size_t cache_len = 0;   /* number of stored terms */
static size_t cache_cap = 0;   /* current cache capacity */

/* ensure the cache can hold at least 'need' elements */
static void ensure_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need)
        new_cap = need;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* check whether a given 64‑bit integer satisfies the pattern */
static bool is_candidate(uint64_t num)
{
    int cnt0 = 0, cnt2 = 0;
    if (num == 0)
        return false;
    uint64_t t = num;
    while (t > 0) {
        int d = t % 5;
        if (d == 0)
            cnt0++;
        else if (d == 2)
            cnt2++;
        t /= 5;
    }
    return cnt0 == 2 && cnt2 == 2;
}

/* static variable to keep the next integer to test */
static uint64_t next_num = 0;

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* out of range, harmless */

    /* initialise cache allocation if necessary */
    if (cache_len == 0) {
        ensure_cap(64);
        cache = (seq_t *)realloc(cache, 64 * sizeof(seq_t));
        cache_len = 0;
        cache_cap = 64;
    }

    /* if the requested term is already cached, return it */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /* generate terms until we reach the requested index */
    uint64_t num = next_num;
    while (1) {
        if (is_candidate(num)) {
            if (cache_len == cache_cap)
                /* grow cache if needed */
                ensure_cap(cache_cap ? cache_cap * 2 : 64);
            cache[cache_len++] = (seq_t)num;
            if ((idx_t)cache_len == n) {
                next_num = num + 1;      /* next start point for future calls */
                return cache[n - 1];
            }
        }
        num++;
    }
}
