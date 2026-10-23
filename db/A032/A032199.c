/*
 * A032199  "CIK" (necklace, indistinct, unlabeled) transform of 3,5,7...
 * https://oeis.org/A032199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{d|n} φ(d) * c(n/d) where φ is Euler's totient,
 *            c(1)=3, c(2)=19, and for k ≥ 3, c(k) = 5·c(k‑1) – 2·c(k‑2) + 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *phi_cache   = NULL;
static seq_t *c_cache     = NULL;
static seq_t *conv_cache  = NULL;
static idx_t  cached_max  = 0;      /* largest n that has been cached */

/* Allocate or grow the internal caches up to 'need' (inclusive). */
static void ensure_cache(idx_t need)
{
    if (need <= cached_max)
        return;

    /* Allocate new space (C++ needs a cast to avoid a conversion warning). */
    size_t new_size = (size_t)(need + 1);                      /* indices 0..need */
    phi_cache  = (seq_t *)realloc(phi_cache,  new_size * sizeof(seq_t));
    c_cache    = (seq_t *)realloc(c_cache,    new_size * sizeof(seq_t));
    conv_cache = (seq_t *)realloc(conv_cache, new_size * sizeof(seq_t));

    /* Initialise new slots to zero. */
    for (idx_t i = cached_max + 1; i <= need; ++i) {
        phi_cache[i]  = 0;
        c_cache[i]    = 0;
        conv_cache[i] = 0;
    }

    /* 1. Compute Euler's totient values up to 'need'. */
    for (idx_t i = 0; i <= (size_t)need; ++i)
        phi_cache[i] = (seq_t)i;   /* φ(i) = i initially */

    for (idx_t i = 2; i <= (size_t)need; ++i) {
        if (phi_cache[i] == (seq_t)i) {               /* i is prime */
            for (idx_t j = i; j <= (size_t)need; j += i)
                phi_cache[j] -= phi_cache[j] / (seq_t)i;
        }
    }

    /* 2. Compute c(k) up to 'need'. */
    if (need >= 1)
        c_cache[1] = (seq_t)3;
    if (need >= 2)
        c_cache[2] = (seq_t)19;
    for (idx_t k = 3; k <= (size_t)need; ++k) {
        /* c(k) = 5*c(k‑1) – 2*c(k‑2) + 4 */
        seq_t t = 5 * c_cache[k-1];
        t -= 2 * c_cache[k-2];
        t += 4;
        c_cache[k] = t;
    }

    /* 3. Compute the convolution S[n] = Σ_{d|n} φ(d) * c(n/d). */
    for (idx_t i = 0; i <= (size_t)need; ++i)
        conv_cache[i] = (seq_t)0;

    for (idx_t d = 1; d <= (size_t)need; ++d) {
        seq_t phi_d = phi_cache[d];
        for (idx_t m = d; m <= (size_t)need; m += d) {
            conv_cache[m] += phi_d * c_cache[m / d];
        }
    }

    cached_max = need;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, return zero */
        return (seq_t)0;
    ensure_cache(n);
    /* a(n) = conv_cache[n] / n */
    return conv_cache[n] / (seq_t)n;
}
