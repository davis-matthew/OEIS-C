/*
 * A138890  Non-Padovan numbers.
 * https://oeis.org/A138890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th natural number that is NOT in the Padovan
 *            sequence (Padovan: P(0)=P(1)=P(2)=1, P(k)=P(k-2)+P(k-3))
 * Range:     u8 n <= 236, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of known Padovan numbers.
   The array grows as needed and is shared across calls. */
static seq_t *pad = NULL;          /* array of Padovan numbers */
static size_t pad_len = 0;          /* number of entries currently stored */
static size_t pad_cap = 0;          /* allocated capacity of the array */

/* Ensure the Padovan array contains all terms ≤ limit.
   The array is extended until its last element is ≥ limit. */
static void pad_generate_until(seq_t limit)
{
    /* Initialize with three 1's if needed. */
    if (pad_len < 3) {
        seq_t one = 1;             /* works for integer or big‑int types */
        while (pad_len < 3) {
            if (pad_cap <= pad_len) {
                size_t new_cap = (pad_cap == 0) ? 8 : pad_cap * 2;
                seq_t *tmp = (seq_t *)realloc(pad, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                pad = tmp;
                pad_cap = new_cap;
            }
            pad[pad_len++] = one;
        }
    }
    /* Extend the array until it reaches the requested limit. */
    while (pad_len == 0 || limit > pad[pad_len - 1]) {
        if (pad_cap <= pad_len) {
            size_t new_cap = (pad_cap == 0) ? 8 : pad_cap * 2;
            seq_t *tmp = (seq_t *)realloc(pad, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            pad = tmp;
            pad_cap = new_cap;
        }
        /* Recurrence: P(k) = P(k-2) + P(k-3). */
        seq_t next = pad[pad_len - 3] + pad[pad_len - 2];
        pad[pad_len++] = next;
    }
}

/* Return the n-th non‑Padovan number (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0; harmless */

    idx_t count_non = 0;             /* how many non‑Padovan numbers found */
    seq_t x = 1;                     /* current natural number being examined */
    idx_t pad_idx = 0;               /* index into the Padovan array */

    /* Ensure the Padovan cache contains at least the first element. */
    pad_generate_until(x);

    while (count_non < n) {
        /* Advance pad_idx to the first Padovan number ≥ x. */
        while (pad_idx < pad_len && pad[pad_idx] < x) pad_idx++;

        /* If x is not a Padovan number, count it. */
        if (!(pad_idx < pad_len && pad[pad_idx] == x)) {
            count_non++;
            if (count_non == n)
                return x;
        }

        ++x;                          /* advance to the next natural number */

        /* Ensure the Padovan cache is long enough for the new x. */
        pad_generate_until(x);
    }

    return 0;  /* unreachable */
}
