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
 * Validated: standard, pass, 81 terms through n = 9850 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seeks numbers k such that d(k)^2 does not divide σ(k) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_divisors.h"
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
        seq = mem_new(seq_t, seq_cap);
        if (!seq) return 0;          /* allocation failure */
    }

    /* generate terms until we reach the requested index */
    while (seq_len < n) {
        uint64_t d = num_divisors(next_k);  /* d(k) */
        seq_t sigma = sigma_of(next_k);          /* σ(k) */
        seq_t dseq = (seq_t)d;
        seq_t sq = dseq * dseq;                  /* d(k)^2 */

        if (sq == 0 || sigma % sq != 0) {        /* condition */
            if (seq_len == seq_cap) {            /* grow cache */
                seq_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, seq, seq_cap);
                if (!tmp) break;                 /* allocation failure */
                seq = tmp;
            }
            seq[seq_len++] = (seq_t)next_k;      /* store term */
        }
        ++next_k;
    }

    return seq[n - 1];   /* 1‑based indexing */
}
