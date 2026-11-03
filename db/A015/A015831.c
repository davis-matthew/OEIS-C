/*
 * A015831  Numbers k such that phi(k + 11) | sigma(k).
 * https://oeis.org/A015831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that φ(k + 11) divides σ(k)
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t next_k = 1;
    static bool initialized = false;

    /* One‑time allocation */
    if (!initialized) {
        terms_cap = 256;
        terms = mem_new(seq_t, terms_cap);
        terms_len = 0;
        next_k   = 1;
        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        idx_t   k   = next_k;                   /* current candidate */
        seq_t  phi = totient_seq((seq_t)(k + 11));  /* φ(k + 11) */
        seq_t  sigma = sigma_of(k);             /* σ(k)   */
        if (phi != 0 && sigma % phi == 0) {
            /* store as seq_t (may be arbitrarily large) */
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = (seq_t)k;
        }
        ++next_k;
    }

    return terms[n - 1];
}
