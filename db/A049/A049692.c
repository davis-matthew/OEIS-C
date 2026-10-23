/*
 * A049692  Numbers k such that the square of the number of divisors of k does
 *          not divide the sum of the divisors of k.
 * https://oeis.org/A049692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seeks numbers k such that d(k)^2 does not divide σ(k) */
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n = 0 */

    /* static cache of generated sequence terms */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next_k = 2;          /* next integer to test */

    /* initialize cache */
    if (seq == NULL) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return 0;          /* allocation failure */
    }

    /* generate terms until we reach the requested index */
    while (seq_len < n) {
        uint64_t d = divisor_count_u64(next_k);  /* d(k) */
        seq_t sigma = sigma_of(next_k);          /* σ(k) */
        seq_t dseq = (seq_t)d;
        seq_t sq = dseq * dseq;                  /* d(k)^2 */

        if (sq == 0 || sigma % sq != 0) {        /* condition */
            if (seq_len == seq_cap) {            /* grow cache */
                seq_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!tmp) break;                 /* allocation failure */
                seq = tmp;
            }
            seq[seq_len++] = (seq_t)next_k;      /* store term */
        }
        ++next_k;
    }

    return seq[n - 1];   /* 1‑based indexing */
}
