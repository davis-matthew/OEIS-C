/*
 * A098456  Expansion of 1/sqrt(1-4*x-64*x^2).
 * https://oeis.org/A098456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A098456:  Expansion of 1/sqrt(1-4*x-64*x^2)
   Recurrence: n*a(n) + (2-4n)*a(n-1) + (64-64n)*a(n-2) = 0
   Initial values: a(0) = 1, a(1) = 2
   Computes the sequence using the recurrence; for 64‑bit
   builds an unsigned __int128 is used to hold intermediate
   products to avoid overflow.  For arbitrary‑precision builds
   the provided seq_mul helper is used.  The implementation
   is compatible with both C11 and C++17. */

#include <stdint.h>
#include "../utils/seq_mul.h"   /* seq_mul(seq_t, seq_t) */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_prev = (seq_t)1;   /* a(n-2) */
    seq_t a_cur  = (seq_t)2;   /* a(n-1) */
    seq_t a_next = a_cur;

    for (idx_t i = 2; i <= n; ++i) {
#ifdef __cplusplus
        /* For big‑int builds, let seq_mul handle large products. */
        seq_t term1 = seq_mul((seq_t)(4*i - 2), a_cur);
        seq_t term2 = seq_mul((seq_t)(64*(i - 1)), a_prev);
        seq_t sum   = term1 + term2;
        a_next = sum / (seq_t)i;
#else
        /* For built‑in integer builds, use unsigned __int128 for
           intermediate products to avoid overflow. */
        unsigned __int128 t1 = (unsigned __int128)(4*i - 2) * a_cur;
        unsigned __int128 t2 = (unsigned __int128)(64*(i - 1)) * a_prev;
        unsigned __int128 s  = t1 + t2;
        a_next = (seq_t)(s / i);
#endif
        a_prev = a_cur;
        a_cur  = a_next;
    }
    return a_cur;
}
