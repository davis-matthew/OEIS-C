/*
 * A045559  Numbers whose factorial has '44' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer k for which (k! /
 *            10^{v_5(k!)}) mod 100 = 44
 * Range:     u8 n <= 11, u16 n <= 3273, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow5_mod100
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The algorithm keeps a running factorial with all factors of 2 and 5 removed.
    It also counts the total number of 2‑factors and 5‑factors processed.
    After each step we reconstruct the value
          prod * 2^{cnt2-cnt5}   (mod 100)
    and test whether it equals 44.  Whenever this happens we store the
    current k in a cache.  The cache is grown lazily so that any
    index n can be answered in O(n) time. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow5_mod100.h"

/* helpers --------------------------------------------------------- */

static seq_t pow2_mod100(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (e) {
        if (e & 1U)
            result = (seq_t)((uint32_t)result * (uint32_t)base % 100);
        base = (seq_t)((uint32_t)base * (uint32_t)base % 100);
        e >>= 1U;
    }
    return result;
}

/* cache ------------------------------------------------------------- */

static seq_t *cache      = NULL;   /* array of terms found so far        */
static size_t cache_len  = 0;      /* number of terms stored             */
static size_t cache_cap  = 0;      /* allocated capacity                 */

static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* unrecoverable memory error      */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* build sequence up to at least 'need' terms --------------------- */

static void build_cache_up_to(idx_t need)
{
    /* These static variables keep the state between calls. */
    static seq_t prod       = 1;   /* product of odd residues mod 100       */
    static idx_t  cnt2      = 0;   /* total number of 2‑factors processed   */
    static idx_t  cnt5      = 0;   /* total number of 5‑factors processed   */
    static idx_t  cur_k     = 0;   /* last integer processed               */

    while (cache_len < need) {
        cur_k++;

        /* strip 2‑factors */
        idx_t x = cur_k;
        while ((x & 1U) == 0U) {
            cnt2++;
            x >>= 1U;
        }

        /* strip 5‑factors */
        while (x % 5U == 0U) {
            cnt5++;
            x /= 5U;
        }

        /* multiply the remaining part modulo 100 */
        prod = (seq_t)((uint32_t)prod * (uint32_t)x % 100);

        /* normalize the amount of surplus 2‑factors */
        long long diff = (long long)cnt2 - (long long)cnt5;
        seq_t val = prod;
        if (diff > 0)
            val = (seq_t)((uint32_t)val * (uint32_t)pow2_mod100((idx_t)diff) % 100);
        else if (diff < 0)
            val = (seq_t)((uint32_t)val * (uint32_t)pow5_mod100((idx_t)(-diff)) % 100);

        if (val == 44U)
            cache_append((seq_t)cur_k);
    }
}

/* sequence function ------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for index 0          */
    build_cache_up_to(n);                 /* ensure we have at least n terms*/
    return cache[n - 1];
}
