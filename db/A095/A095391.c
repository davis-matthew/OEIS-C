/*
 * A095391  a(n) is the least x such that A094892(x)=n.
 * https://oeis.org/A095391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = the least x such that A094892(x)=n, where A094892(x)
 *            counts primes in the interval [210x, 210x+209]
 * Range:     u8 none, u16 none, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_comp_unsigned, set_comp_unsigned
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/is_comp_unsigned.h"
#include "../utils/set_comp_unsigned.h"

/* Global cache for a(n) */
static seq_t *a_cache = NULL;
static idx_t a_cache_len = 0;
static bool init_done = false;

/* One‑time initialisation */
static void init_seq(void)
{
    if (init_done) return;
    init_done = true;

    /* Upper bound to search: covers all known values in the OEIS test set */
    const idx_t MAX_A = 2000000ULL;            /* maximum x to test */
    const idx_t LIM_NUM = 210ULL * (MAX_A + 1);/* maximum number to sieve up to */

    /* odd‑only bitset: numbers 3,5,7,…,LIM_NUM */
    idx_t odd_bits = (LIM_NUM >= 3) ? ((LIM_NUM - 3) >> 1) + 1 : 0;
    size_t bits_bytes = (odd_bits + 7) >> 3;
    unsigned char *bits = (unsigned char *)calloc(bits_bytes, 1);
    if (!bits) abort();

    /* Sieve of Eratosthenes on odd numbers only */
    idx_t sqrt_lim = (idx_t)(sqrt((double)LIM_NUM) + 1);
    for (idx_t i = 3; i <= sqrt_lim; i += 2) {
        if (!is_comp_unsigned(i, bits)) {
            for (idx_t j = i * i; j <= LIM_NUM; j += i << 1)
                set_comp_unsigned(j, bits);
        }
    }

    /* Count primes in each 210‑interval up to MAX_A */
    uint16_t *cnt = (uint16_t *)calloc(MAX_A + 1, sizeof(uint16_t));
    if (!cnt) abort();

    /* prime 2 */
    if (2 <= LIM_NUM) {
        idx_t idx = 2 / 210;
        if (idx <= MAX_A) cnt[idx]++;
    }
    /* iterate over odd numbers */
    for (idx_t num = 3; num <= LIM_NUM; num += 2) {
        if (!is_comp_unsigned(num, bits)) {
            idx_t idx = num / 210;
            if (idx <= MAX_A) cnt[idx]++;
        }
    }

    /* find largest count n to size the cache */
    idx_t max_n = 0;
    for (idx_t i = 0; i <= MAX_A; ++i) {
        if (cnt[i] > max_n) max_n = cnt[i];
    }
    a_cache_len = max_n + 1;
    a_cache = (seq_t *)calloc(a_cache_len, sizeof(seq_t));
    if (!a_cache) abort();

    /* record minimal x for each n */
    for (idx_t i = 0; i <= MAX_A; ++i) {
        idx_t n = cnt[i];
        if (n < a_cache_len && a_cache[n] == 0)
            a_cache[n] = (seq_t)i;
    }

    free(cnt);
    free(bits);
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (!init_done) init_seq();
    if (n >= a_cache_len) return (seq_t)0;
    return a_cache[n];
}
