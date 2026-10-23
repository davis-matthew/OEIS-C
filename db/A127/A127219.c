/*
 * A127219  Even numbers with an even number of partitions.
 * https://oeis.org/A127219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A127219: a(n) = the nth even integer m such that the partition
   function p(m) is even.  Equivalent to the OEIS definition. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return the nth element of the sequence.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    /* ---------- static state ---------------------------------------- */
    static bool initialized = false;
    static uint8_t *parity = NULL;        /* parity[0..m] stored as 0/1 */
    static size_t parity_cap = 0;         /* allocated capacity */
    static idx_t parity_len = 0;          /* highest index computed +1 */

    static seq_t *seq = NULL;            /* sequence values */
    static size_t seq_cap = 0;           /* allocated capacity */
    static idx_t seq_len = 0;            /* number of terms stored */

    static idx_t next_even = 2;          /* next even m to test */

    /* ---------- helper: ensure parity array size -------------------- */
    if (!initialized) {
        parity_cap = 1;
        parity = (uint8_t*)malloc(parity_cap * sizeof(uint8_t));
        parity[0] = 1;                   /* p(0) = 1 (odd) */
        parity_len = 1;
        seq_cap = 16;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        initialized = true;
    }

    while (seq_len < n) {
        /* Grow parity array up to next_even */
        if (next_even >= parity_cap) {
            size_t new_cap = parity_cap * 2;
            while (new_cap <= (size_t)next_even) new_cap *= 2;
            uint8_t *tmp = (uint8_t*)realloc(parity, new_cap * sizeof(uint8_t));
            if (!tmp) abort();            /* allocation failure */
            parity = tmp;
            for (size_t i = parity_cap; i < new_cap; ++i) parity[i] = 0;
            parity_cap = new_cap;
        }

        /* Compute parity[next_even] if not already computed */
        if ((idx_t)parity_len <= next_even) {
            for (idx_t i = parity_len; i <= next_even; ++i) {
                uint8_t val = 0;
                for (idx_t k = 1; ; ++k) {
                    idx_t pent1 = k * (3 * k - 1) / 2;
                    if (pent1 > i) break;
                    val ^= parity[i - pent1];
                    idx_t pent2 = k * (3 * k + 1) / 2;
                    if (pent2 <= i) val ^= parity[i - pent2];
                }
                parity[i] = val;
            }
            parity_len = next_even + 1;
        }

        /* If partition count is even, store the even number */
        if (parity[next_even] == 0) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap * 2;
                seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) abort();        /* allocation failure */
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)next_even;
        }

        next_even += 2;   /* next even number */
    }

    return seq[n - 1];
}
