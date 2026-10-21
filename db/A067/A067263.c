/*
 * A067263  Numbers k such that sigma(k+1) = 5*phi(k).
 * https://oeis.org/A067263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that sigma(k+1) = 5*phi(k)
 * Range:     u8 none, u16 n <= 9, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/phi.h"
#include "../utils/sigma_of.h"

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
        cached = (seq_t *)malloc(cached_cap * sizeof(seq_t));
    }

    while (cached_len < (size_t)n) {
        seq_t sigma = sigma_of(next_k + 1);
        seq_t phi_val = phi(next_k);
        seq_t rhs = (seq_t)5 * phi_val;
        if (sigma == rhs) {
            if (cached_len == cached_cap) {
                cached_cap *= 2;
                cached = (seq_t *)realloc(cached, cached_cap * sizeof(seq_t));
            }
            cached[cached_len++] = next_k;
        }
        next_k++;
    }
    return cached[n - 1];
}
