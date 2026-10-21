/*
 * A228330  Let h(m) denote the sequence whose n-th term is Sum_{k=0..n}
 *          (k+1)^m*T(n,k)^2, where T(n,k) is the Catalan triangle A039598.
 *          This is h(4).
 * https://oeis.org/A228330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n,2*n) * (15*n^3+15*n^2+n-1) /
 *            ((2*n+1)*(4*n-1))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses binomial(idx_t a, idx_t b) from utils and gcd(seq_t a, seq_t b). */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Simple memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else if (i == 1) {
                val = (seq_t)20;
            } else {
                seq_t nn = (seq_t)i;
                /* binomial(4*i, 2*i) */
                seq_t bin = binomial((idx_t)(4 * i), (idx_t)(2 * i));

                /* numerator = 15*n^3 + 15*n^2 + n - 1 */
                seq_t num = (seq_t)15 * nn * nn * nn
                            + (seq_t)15 * nn * nn
                            + nn
                            - (seq_t)1;

                /* denominator = (2*n+1)*(4*n-1) */
                seq_t den = ((seq_t)2 * nn + (seq_t)1)
                            * ((seq_t)4 * nn - (seq_t)1);

                /* Reduce the fraction to avoid overflow */
                seq_t g = gcd(num, den);
                if (g > 1) { num /= g; den /= g; }

                g = gcd(bin, den);
                if (g > 1) { bin /= g; den /= g; }

                /* Final reduction if needed */
                if (den != 1) {
                    g = gcd(bin, den);
                    if (g > 1) { bin /= g; den /= g; }
                    g = gcd(num, den);
                    if (g > 1) { num /= g; den /= g; }
                }

                /* Compute the result */
                val = bin * num / den;
            }

            /* Grow cache if needed */
            if (i >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                while (new_cap <= i) new_cap *= 2;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[i] = val;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
