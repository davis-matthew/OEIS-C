/*
 * A039357  Numbers whose base-9 representation has the same nonzero number of
 *          0's and 1's.
 * https://oeis.org/A039357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 9032, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_zeros_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A039357 – numbers whose base‑9 representation has the same nonzero
   number of 0's and 1's; computed by brute‑force search with memoization. */

#include <stdlib.h>
#include "../utils/count_zeros_ones.h"

static seq_t *seq = NULL;          /* stored terms */
static size_t seq_cap = 0;         /* allocation capacity */
static size_t seq_len = 0;         /* number of populated terms */
static idx_t  last_checked = 1;    /* next candidate to examine */

/* Ensure that seq has room for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq = tmp;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for offset 1, return 0 */

    /* Build terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        /* Search for the next valid number. */
        while (1) {
            idx_t c0, c1;
            count_zeros_ones(last_checked, &c0, &c1);
            if (c0 == c1 && c0 > 0) {
                /* Store the term. */
                ensure_capacity(seq_len + 1);
                seq[seq_len++] = (seq_t)last_checked;
                ++last_checked;   /* advance to avoid duplicate */
                break;
            }
            ++last_checked;
        }
    }

    return seq[(size_t)n - 1];
}
