/*
 * A177982  a(n) = composite number > a(n-1) that contains the n-th composite
 *          as a substring.
 * https://oeis.org/A177982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest composite number > a(n-1) that contains the decimal
   representation of the nth composite number as a proper (non‑whole)
   substring.  No explicit formula is known; the implementation finds
   the terms by incremental search.  The algorithm is O(n) because a(n)
   ≤ 10*n. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Simple primality test for uint64_t values (fast enough for n ≤ 10⁵). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* Convert a uint64_t to a decimal string.  Assumes buf has room for at least 32 chars. */
static void uint64_to_str(uint64_t v, char *buf)
{
    char tmp[32];
    int pos = 0;
    if (v == 0) tmp[pos++] = '0';
    while (v > 0) {
        tmp[pos++] = '0' + (v % 10);
        v /= 10;
    }
    for (int i = 0; i < pos; ++i) buf[i] = tmp[pos - 1 - i];
    buf[pos] = '\0';
}

/* Grow a heap‑allocated array if needed. */
static void grow_if_needed(void **ptr, size_t *cap, size_t elem_size, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
       Cache of computed a(k) values.  cache[i] holds a(i+1) for i≥0.
       ------------------------------------------------------------------ */
    static seq_t *cache = NULL;        /* cache array          */
    static size_t cache_cap = 0;        /* allocated capacity   */
    static idx_t cache_cnt = 0;         /* number of terms cached */

    /* ------------------------------------------------------------------
       List of composite numbers (stored as uint64_t).  comps[i] is the i‑th
       composite (0‑based, comps[0] = 4).  The list is grown on demand.
       ------------------------------------------------------------------ */
    static uint64_t *comps = NULL;      /* composite array      */
    static size_t comps_cap = 0;        /* allocated capacity   */
    static idx_t comps_cnt = 0;         /* number of composites */
    static uint64_t next_comp = 4;      /* next integer to test */

    /* ------------------------------------------------------------------
       Ensure we have at least n terms cached.  If not, generate them.
       ------------------------------------------------------------------ */
    if (cache_cnt < n) {
        /* Grow the cache array. */
        grow_if_needed((void **)&cache, &cache_cap,
                       sizeof(seq_t), n);

        /* Grow the composite list to at least n composites. */
        while (comps_cnt < n) {
            while (true) {
                uint64_t value = next_comp++;
                if (!is_prime_uint64(value)) {
                    grow_if_needed((void **)&comps, &comps_cap,
                                   sizeof(uint64_t), comps_cnt + 1);
                    comps[comps_cnt++] = value;
                    break;
                }
            }
        }

        /* Compute missing a(k) terms. */
        for (idx_t i = cache_cnt; i < n; ++i) {
            uint64_t comp_val = comps[i];             /* nth composite */
            uint64_t prev = (i == 0) ? 0 : (uint64_t)cache[i - 1];
            uint64_t cand = prev + 1;

            /* Convert comp_val once. */
            char comp_buf[32];
            uint64_to_str(comp_val, comp_buf);

            /* Search for the next composite candidate. */
            for (;;) {
                if (cand == comp_val) { cand++; continue; } /* proper substring */
                if (is_prime_uint64(cand)) { cand++; continue; }

                /* Convert cand to string. */
                char cand_buf[32];
                uint64_to_str(cand, cand_buf);

                /* Quick substring test. */
                if (strstr(cand_buf, comp_buf) != NULL) break;
                cand++;
            }
            cache[i] = (seq_t)cand;
        }
        cache_cnt = n;
    }
    return cache[n - 1];
}
