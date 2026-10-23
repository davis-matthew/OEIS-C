/*
 * A165926  a(n) = sum of the square-full quadratic non-residues of n.
 * https://oeis.org/A165926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the numbers ≤ n that are divisible by a prime
 *            square (i.e. have at least one prime squared in their
 *            factorisation) and that are quadratic non‑residues modulo n
 * Range:     u8 n <= 47, u16 n <= 639, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   make_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/make_spf.h"

/* ---------- test whether x has a square factor (i.e. not square‑free) ---------- */
static int has_square_factor(idx_t x, const uint32_t *spf)
{
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        size_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
            if (cnt >= 2) return 1;
        }
    }
    return 0;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* safety for non‑positive input */

    /* 1. build residue table for squares modulo n */
    char *is_res = (char *)malloc((size_t)n);
    if (!is_res) return 0;                  /* allocation failure */
    for (size_t i = 0; i < (size_t)n; ++i) is_res[i] = 0;
    for (size_t i = 0; i < (size_t)n; ++i) {
        size_t r = (i * i) % n;
        is_res[r] = 1;
    }

    /* 2. sieve smallest prime factors up to n */
    uint32_t *spf = make_spf((size_t)n);
    if (!spf) { free(is_res); return 0; }

    /* 3. iterate over 1 … n */
    seq_t sum = 0;
    for (size_t k = 1; k <= (size_t)n; ++k) {
        if (!has_square_factor(k, spf)) continue;          /* not “square‑full” */
        size_t modk = k % n;
        if (!is_res[modk]) sum += (seq_t)k;                 /* quadratic non‑residue */
    }

    free(is_res);
    free(spf);
    return sum;
}
