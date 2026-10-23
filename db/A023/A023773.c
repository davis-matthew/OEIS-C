/*
 * A023773  Metadromes: digits in base 4 are in strict ascending order.
 * https://oeis.org/A023773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Metadromes: digits in base 4 are strictly increasing.  The
 * sequence consists of all numbers whose representation in base 4
 * has digits aₖ < a_{k‑1} < … < a₀ with aₗ ≠ 0 (i.e. the digits form
 * a strictly ascending sequence).  The first 8 terms are
 * 0, 1, 2, 3, 6, 7, 11, 27; no further terms exist.  This function
 * generates those values lazily on the first call and returns 0 for
 * indices beyond the extent of the sequence.  The implementation
 * works for any of the supported seq_t types or arbitrary‑precision
 * types. */
#include <stddef.h>

static seq_t ext_a_cache[8];
static unsigned ext_a_len = 0;

static void build_ext_a_cache(void)
{
    unsigned idx = 0;
    /* term 0 */
    ext_a_cache[idx++] = (seq_t)0;

    /* 1‑digit numbers: 1, 2, 3 */
    for (seq_t d1 = 1; d1 <= 3; ++d1) {
        ext_a_cache[idx++] = d1;
    }

    /* 2‑digit numbers: 12, 13, 23 in base 4 */
    for (seq_t d1 = 1; d1 <= 2; ++d1) {
        for (seq_t d2 = d1 + 1; d2 <= 3; ++d2) {
            ext_a_cache[idx++] = d1 * (seq_t)4 + d2;
        }
    }

    /* 3‑digit numbers: 123 in base 4 */
    ext_a_cache[idx++] = (seq_t)1 * (seq_t)16 + (seq_t)2 * (seq_t)4 + (seq_t)3;

    ext_a_len = idx;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for callers with 0‑based indices */
    if (ext_a_len == 0) build_ext_a_cache();
    if (n <= ext_a_len) return ext_a_cache[n - 1];
    return (seq_t)0; /* no further terms */
}
