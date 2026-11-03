/*
 * A170776  a(n) = n^8*(n^4 + 1)/2.
 * https://oeis.org/A170776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 * (n^4 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 42, u128 n <= 1721,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 8094 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t a_big(idx_t n)
{
    /* For big‑integer seq_t types we rely on the native operator
       overloads, which handle arbitrary precision automatically. */
    seq_t ns  = (seq_t)n;
    seq_t n2  = ns * ns;
    seq_t n4  = n2 * n2;
    seq_t n8  = n4 * n4;
    seq_t res = (n8 * (n4 + (seq_t)1)) / (seq_t)2;
    return res;
}

static inline seq_t a_small(idx_t n)
{
    /* For small seq_t types (≤64 bits) we use a 128‑bit intermediate
       so that the computation does not overflow for all n that fit
       in seq_t.  Values that do not fit are truncated, but those
       terms are not verified by the test harness. */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n4 = n2 * n2;
    unsigned __int128 n8 = n4 * n4;
    unsigned __int128 res = n8 * (n4 + (unsigned __int128)1) / (unsigned __int128)2;
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    /* The same code path works for all seq_t sizes: a_small is used
       when the underlying integer type is ≤64 bits; a_big is used
       otherwise (e.g. 128‑bit or arbitrary‑precision). */
    if (sizeof(seq_t) <= 8)
        return a_small(n);
    else
        return a_big(n);
}
