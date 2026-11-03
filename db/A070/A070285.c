/*
 * A070285  a(n) = n^(n-2) * (n-1)^(n-1).
 * https://oeis.org/A070285
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-2) * (n-1)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 88 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

static inline seq_t mul_seq(seq_t a, seq_t b) {
#ifdef __cplusplus
    /* C++ build – seq_t is an arbitrary‑precision class */
    return a * b;
#else
    /* C build – use 128‑bit intermediate if available to avoid overflow */
#if defined(__SIZEOF_INT128__)
    unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
    return (seq_t)prod;
#else
    return a * b;
#endif
#endif
}

seq_t a(idx_t n)
{
    /* a(n) is defined for n >= 2. For n < 2 return 0 (undefined). */
    if (n < 2) return (seq_t)0;

    /* Compute n^(n-2) and (n-1)^(n-1) using the provided power routine */
    seq_t pow_n  = ipow_idx((seq_t)n,     n-2);  /* n^(n-2) */
    seq_t pow_n1 = ipow_idx((seq_t)(n-1), n-1);  /* (n-1)^(n-1) */

    /* Multiply the two results exactly */
    return mul_seq(pow_n, pow_n1);
}
