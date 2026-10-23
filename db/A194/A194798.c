/*
 * A194798  Numbers k having the same parity as the number of partitions of k.
 * https://oeis.org/A194798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer k such that k and p(k) have the same
 *            parity (p(k) = number of integer partitions of k). The routine
 *            generates the sequence by computing the partition parity via
 *            Euler's pentagonal recurrence (mod 2) and selects the numbers
 *            that satisfy the parity condition
 * Range:     u8 n <= 131, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return the n‑th term of OEIS sequence A194798.
   Input: n ≥ 1.
   Output: the n‑th integer k with the same parity as p(k). */
seq_t a(idx_t n)
{
    /* Static storage for computed terms and parity cache. */
    static seq_t  *values      = NULL;   /* contains the sequence values collected so far */
    static size_t values_cap   = 0;      /* capacity of the values array */
    static size_t values_len   = 0;      /* number of values stored */

    static uint8_t *parity     = NULL;   /* parity[i] = p(i) mod 2 for i from 0…last_checked */
    static size_t  parity_cap  = 0;      /* capacity of the parity array (last index + 1) */

    static idx_t   last_checked = 0;     /* largest k that has been processed */

    /* Initialise the parity cache with p(0)=1. */
    if (parity == NULL) {
        parity = (uint8_t *)malloc(1);
        if (parity == NULL) return 0;           /* allocation failed – unlikely */
        parity[0] = 1;
        parity_cap = 1;
    }

    /* Generate sequence terms until we have at least n of them. */
    while (values_len < n) {
        /* Next candidate integer k. */
        ++last_checked;

        /* Ensure the parity array can store index last_checked. */
        if ((size_t)last_checked >= parity_cap) {
            size_t new_cap = parity_cap ? parity_cap * 2U : 16U;
            if (new_cap <= (size_t)last_checked)
                new_cap = (size_t)last_checked + 1U;
            uint8_t *tmp = (uint8_t *)realloc(parity, new_cap);
            if (tmp == NULL) return 0;             /* allocation failed */
            parity = tmp;
            parity_cap = new_cap;
        }

        /* Compute p(last_checked) mod 2 via Euler's pentagonal recurrence.
           Mod 2 the alternating signs disappear, so we just XOR the relevant terms. */
        uint8_t p_val = 0;
        for (idx_t m = 1; ; ++m) {
            idx_t g1 = m * (3 * m - 1) / 2;
            if (g1 > last_checked) break;
            p_val ^= parity[last_checked - g1];
            idx_t g2 = m * (3 * m + 1) / 2;
            if (g2 <= last_checked)
                p_val ^= parity[last_checked - g2];
        }
        parity[last_checked] = p_val;

        /* If k and p(k) have the same parity, keep k in the sequence. */
        if (((last_checked & 1) == (p_val & 1))) {
            /* Grow the values array if necessary. */
            if (values_len >= values_cap) {
                size_t new_cap = values_cap ? values_cap * 2U : 16U;
                seq_t *tmp = (seq_t *)realloc(values,
                                             new_cap * sizeof(seq_t));
                if (tmp == NULL) return 0;         /* allocation failed */
                values = tmp;
                values_cap = new_cap;
            }
            values[values_len++] = (seq_t)last_checked;
        }
    }

    /* Return the requested term. */
    return values[n - 1];
}
