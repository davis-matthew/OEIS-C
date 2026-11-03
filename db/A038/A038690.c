/*
 * A038690  a(n)^2 is smallest square containing the string 'n'.
 * https://oeis.org/A038690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 207, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9286 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
/* Sequence A038690: a(n)^2 is the smallest square containing the
   decimal string of n as a contiguous substring. */
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

static seq_t *cache = NULL;      /* cache for computed values */
static idx_t cache_cap = 0;       /* current capacity of cache */

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    idx_t new_cap = (n + 1) * 2;
    seq_t *new_cache = mem_new(seq_t, new_cap);
    if (new_cache == NULL)
        return; /* out of memory – unlikely in tests */
    if (cache != NULL) {
        mem_copy(new_cache, cache, cache_cap * sizeof(seq_t));
        mem_free(cache);
    }
    mem_set(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* a(0) = 0 by definition */
    ensure_cache(n);

    /* Return cached value if available */
    if (cache[n] != 0)
        return cache[n];

    /* Convert n to string */
    char n_str[32];
    snprintf(n_str, sizeof(n_str), "%lu", (unsigned long)n);
    size_t n_len = strlen(n_str);

    /* Brute‑force search for the minimal m */
    for (seq_t m = 1; ; ++m) {
        seq_t sq = m * m;

        /* Convert square to string – allocate a safe buffer */
        char sq_str[128];
        size_t sq_len;
        seq_to_str(sq, sq_str, &sq_len);

        /* Ensure null termination for strstr */
        if (sq_len < sizeof(sq_str))
            sq_str[sq_len] = '\0';
        else
            sq_str[sizeof(sq_str) - 1] = '\0';

        if (strstr(sq_str, n_str) != NULL) {
            cache[n] = m;
            return m;
        }
    }
}
