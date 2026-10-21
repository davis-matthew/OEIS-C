/*
 * A209974  a(n) = A209973(n)/4.
 * https://oeis.org/A209974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*A126246(n) - A000010(n); a(0) = 0, a(1) = 0
 *            where A000010(n) = φ(n) (Euler's totient) and A126246(n) = {
 *            φ(n), if n is odd; 2φ(n), if n is even and the exponent of 2 in
 *            n is 1; 3φ(n)/2, if n is even and the exponent of 2 in n ≥ 2 }
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* compute Euler's totient φ(n) */
static seq_t totient(idx_t n) {
    seq_t result = (seq_t)n;
    idx_t tmp = n;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1) {
        result -= result / tmp;
    }
    return result;
}

/* memoized computation of a(n) */
seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_used = 0;   /* number of computed values = cache_used */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache_used = 2;
    }

    /* extend cache if required */
    if (n >= cache_used) {
        if ((size_t)(n + 1) > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        for (idx_t i = cache_used; i <= n; ++i) {
            seq_t phi_i = totient(i);
            seq_t A126;

            if (i & 1) {                /* odd n */
                A126 = phi_i;
            } else {                    /* even n */
                /* count exponent of 2 in i */
                idx_t temp = i;
                idx_t e = 0;
                while ((temp & 1) == 0) {
                    ++e;
                    temp >>= 1;
                }
                if (e == 1) {           /* 2 * φ(n) */
                    A126 = phi_i * (seq_t)2;
                } else {                /* 3φ(n)/2 */
                    A126 = phi_i * (seq_t)3 / (seq_t)2;
                }
            }

            seq_t prev = cache[i - 1];
            cache[i] = prev + A126 * (seq_t)2 - phi_i;
        }
        cache_used = n + 1;
    }

    return cache[n];
}
