/*
 * A067263  Numbers k such that sigma(k+1) = 5*phi(k).
 * https://oeis.org/A067263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that sigma(k+1) = 5*phi(k)
 * Range:     u8 none, u16 n <= 9, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/totient.h"
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
    /* cached terms of the sequence */
    static seq_t *cached = NULL;
    static size_t cached_len = 0;
    static size_t cached_cap = 0;
    /* next k to test */
    static idx_t next_k = 1;

    if (cached == NULL) {
        cached_cap = 128;
        cached = mem_new(seq_t, cached_cap);
    }

    while (cached_len < (size_t)n) {
        seq_t sigma = sigma_of(next_k + 1);
        seq_t phi_val = totient(next_k);
        seq_t rhs = (seq_t)5 * phi_val;
        if (sigma == rhs) {
            if (cached_len == cached_cap) {
                cached_cap *= 2;
                cached = mem_resize(seq_t, cached, cached_cap);
            }
            cached[cached_len++] = next_k;
        }
        next_k++;
    }
    return cached[n - 1];
}
