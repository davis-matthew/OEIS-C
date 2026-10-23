/*
 * A044214  Numbers k such that string 3,3 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that the base‑8 *
 *            representation of k contains the substring "33" while k-1 does
 *            not
 * Range:     u8 n <= 4, u16 n <= 1010, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, has33_octal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/has33_octal.h"

seq_t a(idx_t n)
{
    /* Static state so that non‑increasing calls are fast. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static seq_t next_k = 1;   /* next candidate to test */

    if (n == 0) return 0;   /* Undefined for the offset, but safe. */

    /* Generate terms until we have the desired index. */
    while (terms_len < n) {
        if (has33_octal(next_k) && !has33_octal(next_k - 1)) {
            /* Ensure enough space and store the term. */
            grow_seq(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = next_k;
        }
        next_k++;
    }
    return terms[n - 1];
}
