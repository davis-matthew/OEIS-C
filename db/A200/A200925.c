/*
 * A200925  Numbers k such that Omega(k) = Omega(k - Omega(k)), where Omega =
 *          A001222.
 * https://oeis.org/A200925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that Omega(k) = Omega(k – Omega(k))
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search the integers and keep a memoised vector of the found terms. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/grow_seq_array.h"

/* Return the n-th term of sequence A200925 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;       /* undefined for n < 1, return 0 */

    /* Static state: cache of found terms and next k to inspect. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t next_k = 1;    /* next integer to test */

    /* Extend the cache until we have at least n terms. */
    while (terms_len < (size_t)n) {
        seq_t omega_k = bigomega(next_k);

        /* If k <= Omega(k), then k – Omega(k) <= 0, skip. */
        if (next_k <= (idx_t)omega_k) {
            ++next_k;
            continue;
        }

        idx_t diff = next_k - (idx_t)omega_k;
        seq_t omega_diff = bigomega(diff);

        if (omega_k == omega_diff) {
            /* Store k in the cache, growing it if necessary. */
            grow_seq_array(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = next_k;
        }

        ++next_k;
    }

    return terms[(size_t)n - 1];
}
