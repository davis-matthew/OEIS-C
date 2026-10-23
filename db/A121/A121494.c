/*
 * A121494  Composite numbers k such that tau(k) = tau(2k+1).
 * https://oeis.org/A121494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 9565, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A121494: composite k such that τ(k)=τ(2k+1)
   (computed by a simple incremental search). */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Offset 1 – return 0 for n == 0, though this case is unused. */
    if (n == 0) return 0;

    /* Static state for incremental generation. */
    static uint64_t *terms      = NULL;  /* terms as 64‑bit integers */
    static size_t terms_len    = 0;      /* number of stored terms   */
    static size_t terms_cap    = 0;      /* capacity of the array   */
    static uint64_t next_k      = 4;     /* next candidate integer  */

    /* Generate terms until enough have been found. */
    while (terms_len < (size_t)n) {
        uint64_t k = next_k++;

        /* Skip non‑composites: τ(p)=2 for a prime. */
        uint64_t tau_k = divisor_count_u64(k);
        if (tau_k < 3) continue;

        uint64_t tau_2k1 = divisor_count_u64(2 * k + 1);

        if (tau_k == tau_2k1) {
            /* Ensure capacity. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(terms, new_cap * sizeof(uint64_t));
                if (!tmp) abort();  /* out of memory */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = k;
        }
    }

    /* Return the requested term, cast to the build‑specific seq_t type. */
    return (seq_t)terms[n - 1];
}
