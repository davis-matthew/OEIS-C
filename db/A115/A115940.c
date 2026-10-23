/*
 * A115940  Pandigital (meaning every digit appears exactly once) triangular
 *          numbers.
 * https://oeis.org/A115940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A115940 – Pandigital triangular numbers by searching triangular
   numbers for the pandigital property.  The sequence has 82 terms,
   all of which are 10‑digit numbers not exceeding 9876543210. */

#include <stdbool.h>

/* Check whether a value consists of each decimal digit 0‑9 exactly once. */
static bool is_pandigital(seq_t x)
{
    unsigned char cnt[10] = {0};
    seq_t t = x;
    while (t > 0) {
        seq_t d = t % 10;            /* digit 0‑9 */
        if (d > 9) return false;     /* safety check */
        if (++cnt[(int)d] > 1) return false; /* duplicate */
        t /= 10;
    }
    /* All ten digits must have appeared exactly once. */
    for (int i = 0; i < 10; ++i)
        if (cnt[i] != 1) return false;
    return true;
}

/* Static cache for the 82 pandigital triangular numbers. */
static seq_t _pandigital_tris[82];
static idx_t _stored = 0;

/* Initialise the cache lazily. */
static void _init_cache(void)
{
    if (_stored) return;
    idx_t k = 1;
    while (_stored < 82) {
        seq_t tri = (seq_t)k * ((seq_t)k + 1) / 2;
        if (is_pandigital(tri))
            _pandigital_tris[_stored++] = tri;
        ++k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 */
    _init_cache();
    if (n <= _stored)
        return _pandigital_tris[n - 1];
    return 0;                      /* n beyond the known 82 terms */
}
