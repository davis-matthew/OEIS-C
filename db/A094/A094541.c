/*
 * A094541  Numerator of Product_{k=0..n} ((2*k+1)/(2*k+2))^((-1)^t(k)) where
 *          t(k)=A010060(k) (Thue-Morse sequence).
 * https://oeis.org/A094541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 23, u64 n <= 53, u128 n <= 93,
 *            big any n
 * Helpers:   parity_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A094541 – Numerator of 
 *   ∏_{k=0..n} ((2k+1)/(2k+2))^{(-1)^t(k)}, 
 * where t(k) = A010060(k) (Thue–Morse sequence).
 * The function returns a(n) for any 0 ≤ n.
 * 
 * Algorithm: maintain the current reduced fraction num/den.  
 * For each k, multiply by (2k+1)/(2k+2) if t(k)=0, otherwise by (2k+2)/(2k+1).  
 * Cross‑reduce the factors before multiplying to keep the values small.  
 * Memoise the numerators so repeated calls are O(1) after initial precomputation.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/parity_uint64.h"

/* Greatest‑common divisor for seq_t types (works for all integer‑like seq_t). */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Static cache of numerators.  The denominator is not stored because we only need the
 * numerator of the reduced product.  The current fraction is implicitly known
 * from the cached numerators. */
static seq_t *cache = NULL;      /* array of numerators */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms stored */

/* Current numerator/denominator of the ongoing product.  They are maintained
 * in reduced form; cache[len] holds the numerator after processing up to
 * that index. */
static seq_t cur_num = 1;        /* numerator of the product up to the last processed k */
static seq_t cur_den = 1;        /* denominator of the product */

/* Initialise the cache on first use. */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;  /* allocation failure – caller will get 0 */
    }
}

/* Ensure the cache can store at least 'need' entries. */
static void resize_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 128;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;  /* keep old buffer if realloc fails */
    cache = tmp;
    cache_cap = new_cap;
}

/*
 * Returns the numerator a(n) of the sequence.
 * The function is thread‑unsafe because of the static cache, but that matches
 * the typical usage of sequence accessor functions in this environment.
 */
seq_t a(idx_t n)
{
    init_cache();
    if (cache == NULL) return (seq_t)0;  /* allocation failure */

    /* If we already computed up to n, simply return the cached value. */
    if (cache_len > n) return cache[n];

    /* Continue computing from the last stored term up to n. */
    for (idx_t k = cache_len; k <= n; ++k) {
        uint64_t kk = (uint64_t)k;
        int par = parity_uint64(kk);                /* 0 → +1, 1 → –1 */

        seq_t num_factor = (seq_t)(2ULL * kk + 1ULL);   /* 2k+1 */
        seq_t den_factor = (seq_t)(2ULL * kk + 2ULL);   /* 2k+2 */

        if (par == 0) {      /* exponent +1 : multiply by (2k+1)/(2k+2) */
            seq_t g1 = seq_gcd(cur_num, den_factor);
            cur_num /= g1;
            den_factor /= g1;

            seq_t g2 = seq_gcd(cur_den, num_factor);
            cur_den /= g2;
            num_factor /= g2;

            cur_num *= num_factor;
            cur_den *= den_factor;
        } else {             /* exponent –1 : multiply by (2k+2)/(2k+1) */
            seq_t g1 = seq_gcd(cur_num, num_factor);
            cur_num /= g1;
            num_factor /= g1;

            seq_t g2 = seq_gcd(cur_den, den_factor);
            cur_den /= g2;
            den_factor /= g2;

            cur_num *= den_factor;
            cur_den *= num_factor;
        }

        /* Final reduction to keep the fraction fully reduced. */
        seq_t g = seq_gcd(cur_num, cur_den);
        if (g > (seq_t)1) {
            cur_num /= g;
            cur_den /= g;
        }

        /* Store the new numerator. */
        resize_cache(cache_len + 1);
        cache[cache_len++] = cur_num;
    }

    return cache[n];
}
