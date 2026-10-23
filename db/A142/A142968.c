/*
 * A142968  Fifth column (m=4) of triangle A142963 divided by 16=2^4.
 * https://oeis.org/A142968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A142963(n+5,3)/16 Recurrence (order 15): a(n) = 35a(n-1)
 *            – 560a(n-2) + 5432a(n-3) – 35714a(n-4) + 168542a(n-5) –
 *            589632a(n-6) + 1556776a(n-7) – 3126949a(n-8) + 4777591a(n-9) –
 *            5506936a(n-10) + 4703032a(n-11) – 2881136a(n-12) +
 *            1195632a(n-13) – 300672a(n-14) + 34560a(n-15) Initial values:
 *            a...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* coefficients for the recurrence, absolute values */
static const seq_t coeff[15] = {
    (seq_t)35,     (seq_t)560,     (seq_t)5432,     (seq_t)35714,
    (seq_t)168542, (seq_t)589632, (seq_t)1556776, (seq_t)3126949,
    (seq_t)4777591, (seq_t)5506936, (seq_t)4703032, (seq_t)2881136,
    (seq_t)1195632, (seq_t)300672,  (seq_t)34560
};

/* initial values a(0) … a(15) */
static const seq_t init_terms[16] = {
    (seq_t)1,          (seq_t)179,          (seq_t)5280,
    (seq_t)82610,      (seq_t)919615,       (seq_t)8284857,
    (seq_t)64730022,   (seq_t)457217400,    (seq_t)2999230965,
    (seq_t)18608607535,(seq_t)110625457964,(seq_t)636103699038,
    (seq_t)3562753619915,(seq_t)19541111960965,
    (seq_t)105392471360850,(seq_t)560747327119908
};

/* memoisation storage */
static seq_t *cache      = NULL;
static idx_t  cache_used = 0;
static idx_t  cache_cap  = 0;

/* enlarge the cache if necessary */
static void ensure_capacity(idx_t needed) {
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();                /* allocation failure */
    cache   = new_ptr;
    cache_cap = new_cap;
}

/* initialise cache with the first 16 terms */
static void init_cache(void) {
    if (cache_used > 0) return;           /* already initialised */
    ensure_capacity(16);
    for (idx_t i = 0; i < 16; ++i) cache[i] = init_terms[i];
    cache_used = 16;
}

/* sequence function */
seq_t a(idx_t n) {
    init_cache();
    if (n < cache_used) return cache[n];

    ensure_capacity(n + 1);

    for (idx_t i = cache_used; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* positive contribution: even j (0,2,4,…,14) */
        sum_pos += coeff[0]  * cache[i-1];
        sum_pos += coeff[2]  * cache[i-3];
        sum_pos += coeff[4]  * cache[i-5];
        sum_pos += coeff[6]  * cache[i-7];
        sum_pos += coeff[8]  * cache[i-9];
        sum_pos += coeff[10] * cache[i-11];
        sum_pos += coeff[12] * cache[i-13];
        sum_pos += coeff[14] * cache[i-15];

        /* negative contribution: odd j (1,3,5,…,13) */
        sum_neg += coeff[1]  * cache[i-2];
        sum_neg += coeff[3]  * cache[i-4];
        sum_neg += coeff[5]  * cache[i-6];
        sum_neg += coeff[7]  * cache[i-8];
        sum_neg += coeff[9]  * cache[i-10];
        sum_neg += coeff[11] * cache[i-12];
        sum_neg += coeff[13] * cache[i-14];

        cache[i] = sum_pos - sum_neg;
    }
    cache_used = n + 1;
    return cache[n];
}
