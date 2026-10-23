/*
 * A140490  Trajectory of 1 under repeated application of the map: n -> n +
 *          third-smallest number that does not divide n.
 * https://oeis.org/A140490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   third_non_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A140490 – Trajectory of 1 under n -> n + 3rd‑smallest non‑divisor of n
 *
 * The sequence is defined by  
 *   a(1) = 1
 *   a(n) = a(n‑1) + third_non_divisor(a(n‑1))  for n ≥ 2
 * where third_non_divisor(m) is the third smallest positive integer
 * that does not divide m.
 *
 * The first 24 terms are: 1,5,9,14,19,23,27,32,38,43,47,51,56,62,67,71,
 * 75,81,86,91,95,99,104,110,...
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/third_non_divisor.h"

/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
/* Sequence storage.  We allocate a fixed-size array that is large
 * enough for the benchmarks (n up to ~10000).  The extra space
 * is harmless and keeps the implementation simple for both C
 * and C++ (no dynamic allocation of seq_t objects). */
#define SEQ_CAP 200000   /* > maximum n that will be requested */

static seq_t seq[SEQ_CAP + 1];   /* 1‑based */
static size_t seq_len = 0;       /* last computed index, 0 if none */

/* ---------------------------------------------------------------------- */
/* Main sequence function.  It lazily computes entries up to n if
 * necessary.  The function is safe for any order of calls because
 * once a value has been computed it is never recomputed. */
seq_t a(idx_t n)
{
    /* initialise the first term on first entry */
    if (seq_len == 0) {
        seq[1] = (seq_t)1;
        seq_len = 1;
    }

    /* return immediately if already computed */
    if ((size_t)n <= seq_len)
        return seq[(size_t)n];

    /* extend array if requested n exceeds the fixed capacity */
    if ((size_t)n > SEQ_CAP)
        return (seq_t)0;  /* safety: in practice n will not exceed SEQ_CAP */

    /* compute missing terms up to n */
    for (size_t i = seq_len + 1; i <= (size_t)n; ++i) {
        seq_t prev = seq[i - 1];
        seq_t add  = third_non_divisor(prev);
        seq[i] = prev + add;
    }
    seq_len = (size_t)n;
    return seq[(size_t)n];
}
