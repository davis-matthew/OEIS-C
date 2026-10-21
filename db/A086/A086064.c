/*
 * A086064  In decimal representation: smallest k>1 such that n is a substring
 *          of n*k.
 * https://oeis.org/A086064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest k>1 such that the decimal representation of n is
 *            a substring of the decimal representation of n*k. (1 < a(n) ≤
 *            10)
 * Range:     u8 n <= 25, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   seq_to_dec_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_to_dec_str.h"

/* External function requested by the build system */
seq_t a(idx_t n)
{
    /* Buffer for decimal strings (6 digits for n*k ≤ 100 000, plus NUL) */
    char buf_n[8], buf_p[12];
    size_t len_n, len_p;

    /* String for n */
    seq_to_dec_str((seq_t)n, buf_n, &len_n);

    /* Test k = 2 … 10 */
    for (int k = 2; k <= 10; ++k) {
        seq_t prod = ((seq_t)n) * (seq_t)k;
        seq_to_dec_str(prod, buf_p, &len_p);
        /* strstr expects NUL‑terminated strings; both buffers are NUL‑terminated. */
        if (strstr(buf_p, buf_n) != NULL)
            return (seq_t)k;
    }

    /* Should never happen: by definition a(n) ≤ 10 for all n. */
    return (seq_t)11;
}
