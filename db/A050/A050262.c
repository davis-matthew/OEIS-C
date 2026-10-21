/*
 * A050262  G.f.: ( 1 + x + x^2 - x^3 - x^4 - sqrt( 1 - 2 x - 5 x^2 - 4 x^3 -
 *          3 x^4 - 4 x^5 - x^6 + 2 x^7 + x^8 ) ) / ( 2 x (1 + x) ).
 * https://oeis.org/A050262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A050262: D-finite recurrence
   (n-2)*a(n) + (-3+3*n)*a(n+1) + (n-2)*a(n+2) + (-11-5*n)*a(n+3)
   + (-22-7*n)*a(n+4) + (-34-7*n)*a(n+5) + (-57-9*n)*a(n+6)
   + (-52-7*n)*a(n+7) + (-7-n)*a(n+8) + (10+n)*a(n+9) = 0
   initial values: a(0)=1,a(1)=1,a(2)=1,a(3)=4,a(4)=10,a(5)=27,a(6)=79,
   a(7)=238,a(8)=736 */
#include <stdlib.h>

/* Returns a(n) for OEIS A050262 */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initial values */
    const seq_t init[9] = {1,1,1,4,10,27,79,238,736};

    /* allocate or grow cache */
    if (cache == NULL) {
        cache_size = 9;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 9; ++i) cache[i] = init[i];
    }
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed, return 0 */
        cache = tmp;
        /* compute new terms */
        for (idx_t k = old_size; k <= n; ++k) {
            /* k >= 9 because we already have values 0..8 */
            idx_t base = k - 9;          /* n in recurrence */
            /* signed weights for the 9 terms */
            int64_t w0 = (int64_t)base - 2;
            int64_t w1 = 3 * (int64_t)base - 3;
            int64_t w2 = (int64_t)base - 2;
            int64_t w3 = -11 - 5 * (int64_t)base;
            int64_t w4 = -22 - 7 * (int64_t)base;
            int64_t w5 = -34 - 7 * (int64_t)base;
            int64_t w6 = -57 - 9 * (int64_t)base;
            int64_t w7 = -52 - 7 * (int64_t)base;
            int64_t w8 = -7  - (int64_t)base;
            int64_t denom = 10 + (int64_t)base;

            /* compute numerator = sum_{i=0..8} abs(wi)*a(base+i) with
               signs handled by subtracting sums where wi>0             */
            if (sizeof(seq_t) <= 8) {
                /* use 128-bit intermediates to avoid overflow */
                unsigned __int128 sum_plus = 0, sum_minus = 0;
                int64_t ws[9] = {w0,w1,w2,w3,w4,w5,w6,w7,w8};
                for (int i = 0; i < 9; ++i) {
                    seq_t term = cache[base + i];
                    int64_t w = ws[i];
                    unsigned __int128 absw = w < 0 ? (unsigned __int128)(-w)
                                                  : (unsigned __int128)w;
                    unsigned __int128 prod = absw * (unsigned __int128)term;
                    if (w < 0) sum_plus += prod; else sum_minus += prod;
                }
                unsigned __int128 numer = sum_plus - sum_minus;
                cache[k] = (seq_t)(numer / (unsigned __int128)denom);
            } else {
                /* arbitrary-precision seq_t version */
                seq_t sum_plus = (seq_t)0, sum_minus = (seq_t)0;
                int64_t ws[9] = {w0,w1,w2,w3,w4,w5,w6,w7,w8};
                for (int i = 0; i < 9; ++i) {
                    seq_t term = cache[base + i];
                    int64_t w = ws[i];
                    seq_t absw = (seq_t)(w < 0 ? -w : w);
                    seq_t prod = term * absw;
                    if (w < 0) sum_plus += prod; else sum_minus += prod;
                }
                seq_t numer = sum_plus - sum_minus;
                seq_t denom_seq = (seq_t)denom;
                cache[k] = numer / denom_seq;
            }
        }
    }
    return cache[n];
}
