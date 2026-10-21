/*
 * A092883  Number of ordered 5-multiantichains on an n-set.
 * https://oeis.org/A092883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32^n - 20*24^n + 60*20^n + 20*18^n + 10*17^n - 100*16^n -
 *            120*15^n + 150*14^n + 120*13^n - 360*12^n + 20*11^n + 480*10^n +
 *            80*9^n - 360*8^n + 120*7^n - 720*6^n + 720*5^n + 250*4^n -
 *            500*3^n + 150*2^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* fast exponentiation for seq_t (works for native and big‑integer types) */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        if (exp > 1) base *= base;
        exp >>= 1;
    }
    return result;
}

/* the 20 roots of the characteristic polynomial */
static const idx_t roots[20] =
{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 24, 32};

/* terms of the closed formula: base^n * coefficient */
static const struct { idx_t base; long long coeff; } terms[21] = {
    {32,   1},
    {24, -20},
    {20,  60},
    {18,  20},
    {17,  10},
    {16, -100},
    {15, -120},
    {14, 150},
    {13, 120},
    {12, -360},
    {11,  20},
    {10, 480},
    { 9,   80},
    { 8, -360},
    { 7, 120},
    { 6, -720},
    { 5, 720},
    { 4, 250},
    { 3, -500},
    { 2, 150}
};

/* cache for computed a(n) values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of valid entries in cache */
static idx_t cache_cap = 0;    /* allocated capacity */

/* elementary symmetric sums e[0..20] of the roots */
static seq_t e[21];
static bool e_computed = false;

static void compute_symmetric_sums(void)
{
    if (e_computed) return;
    e[0] = 1;
    for (int i = 1; i <= 20; ++i) e[i] = 0;
    for (int i = 0; i < 20; ++i) {
        idx_t val = roots[i];
        for (int k = i + 1; k > 0; --k) {
            e[k] += e[k - 1] * val;
        }
    }
    e_computed = true;
}

/* initialise cache with first 20 terms using the closed formula */
static void init_cache(void)
{
    if (cache != NULL) return;
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    for (idx_t n = 0; n < 20; ++n) {
        seq_t pos = 0, neg = 0;
        for (int t = 0; t < 21; ++t) {
            seq_t pow = pow_seq((seq_t)terms[t].base, n);
            long long coeff = terms[t].coeff;
            if (coeff >= 0)
                pos += pow * (seq_t)coeff;
            else
                neg += pow * (seq_t)(-coeff);
        }
        cache[n] = pos - neg;
    }
    cache_size = 20;
}

/* extend cache up to at least index n */
static void ensure_terms(idx_t n)
{
    if (n < cache_size) return;

    if (cache == NULL) init_cache();
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) { cache = tmp; cache_cap = new_cap; }
    }

    compute_symmetric_sums();

    while (cache_size <= n) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 20; ++k) {
            seq_t term = cache[cache_size - k] * e[k];
            if (k & 1) sum += term;   /* odd k -> add */
            else        sum -= term;   /* even k -> subtract */
        }
        cache[cache_size++] = sum;
    }
}

/* required function */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[n];
}
