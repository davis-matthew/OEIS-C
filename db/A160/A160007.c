/*
 * A160007  Deficient numbers more than 1 unit away from their predecessors.
 * https://oeis.org/A160007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6160, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A160007: Deficient numbers more than 1 unit away from their predecessors.
   The function lazily generates terms by scanning integers, testing for deficiency
   via sigma_of, keeping the previous deficient number, and recording those
   whose gap exceeds 1. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static state for memoisation */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static idx_t seq_len = 0;
    static idx_t next_test = 1;
    static idx_t prev_def = 0;

    while (seq_len < n)
    {
        idx_t m = next_test;
        /* deficiency test: sum of proper divisors < m */
        seq_t sigma = sigma_of(m);
        seq_t two_m = (seq_t)m;
        two_m += two_m;                /* 2*m */
        if (sigma < two_m)              /* m is deficient */
        {
            if (m - prev_def > (idx_t)1)
            {
                if (seq_len >= seq_cap)
                    grow_seq(&seq, &seq_cap, seq_len + 1);
                seq[seq_len++] = (seq_t)m;
            }
            prev_def = m;
        }
        ++next_test;
    }

    return seq[n - 1];
}
