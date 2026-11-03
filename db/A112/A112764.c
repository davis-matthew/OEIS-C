/*
 * A112764  Greatest prime factor of the n-th 5-smooth number.
 * https://oeis.org/A112764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest_prime_factor(A051037(n))
 * Range:     u8 n <= 50, u16 n <= 607, u32 n <= 2783, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9523 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdalign.h>
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* Helper: safe multiplication using 128‑bit intermediate when
   available and seq_t is a built‑in integer.  For arbitrary‑precision
   types the plain * operator is used. */
static inline seq_t mul128(seq_t a, seq_t b)
{
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    return (seq_t)((unsigned __int128)(unsigned long long)a * b);
#else
    return a * b;
#endif
}

/* Greatest prime factor of an unsigned integer */
static seq_t greatest_prime_factor(seq_t value)
{
    seq_t max_factor = 1;
    seq_t n = value;

    for (seq_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            max_factor = d;
            while (n % d == 0) {
                n /= d;
            }
        }
    }
    if (n > 1) {
        max_factor = n;
    }
    return max_factor;
}

seq_t a(idx_t n)
{
    /* Guard against zero, though n should be ≥1 */
    if (n == 0) return (seq_t)0;

    seq_t *H = seq_malloc(n);
    H[0] = (seq_t)1;            /* first 5‑smooth number */

    idx_t i2 = 0, i3 = 0, i5 = 0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t v2 = mul128(H[i2], (seq_t)2);
        seq_t v3 = mul128(H[i3], (seq_t)3);
        seq_t v5 = mul128(H[i5], (seq_t)5);

        seq_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;
        H[k] = next;

        if (v2 == next) ++i2;
        if (v3 == next) ++i3;
        if (v5 == next) ++i5;
    }

    return greatest_prime_factor(H[n - 1]);
}
