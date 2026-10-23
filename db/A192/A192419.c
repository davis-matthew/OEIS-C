/*
 * A192419  Smallest k such that 1^3, 2^3, 3^3,... n^3 are distinct modulo k.
 * https://oeis.org/A192419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the discriminator of the cubic sequence:
   smallest k such that 1^3, 2^3, …, n^3 are pairwise distinct modulo k. */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* malloc, realloc, free, memset */

seq_t a(idx_t n)
{
    /* Reusable buffer for remainder checking. */
    static unsigned char *seen = NULL;
    static size_t seen_cap = 0;

    for (idx_t k = 1;; ++k) {
        /* k is small enough to fit in size_t for all intended n. */
        size_t ks = (size_t)k;
        if (ks > seen_cap) {
            /* Grow the buffer once per new maximum k. */
            unsigned char *tmp = (unsigned char *)realloc(seen, ks);
            if (!tmp) { free(seen); return 0; } /* OOM – unlikely */
            seen = tmp;
            seen_cap = ks;
        }
        /* Reset only the part of the buffer that will be used. */
        memset(seen, 0, ks);

        seq_t modulus = (seq_t)k;
        bool conflict = false;

        for (idx_t i = 1; i <= n; ++i) {
            seq_t ii = (seq_t)i;
            seq_t cube = ii * ii * ii;   /* i^3 */
            seq_t r_seq = cube % modulus;
            size_t r = (size_t)r_seq;   /* r < k, fits in size_t */

            if (seen[r]) { conflict = true; break; }
            seen[r] = 1;
        }

        if (!conflict) return k;   /* k is the minimal discriminator */
    }
}
