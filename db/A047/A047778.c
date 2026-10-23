/*
 * A047778  Concatenation of the first n numbers in binary (converted to base
 *          10).
 * https://oeis.org/A047778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1,
   for n>1   a(n)=a(n‑1)*2^(1+floor(log2(n))) + n
   This function implements the recurrence using only integer
   arithmetic and no dynamic allocations, valid for both the
   fixed‑width and arbitrary‑precision builds. */

#include <stddef.h>
#include <stdint.h>

/* Compute floor(log2(v)) for an unsigned 64‑bit integer. */
static inline idx_t floor_log2_uint64(idx_t v) {
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* Compute 2^exp as seq_t by repeated left shifts. */
static inline seq_t pow2_u(idx_t exp) {
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r <<= 1;
    return r;
}

/* Return the n-th term (n≥1) of OEIS A047778. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but safe default */

    seq_t res = 1;                 /* a(1) = 1 */

    for (idx_t k = 2; k <= n; ++k) {
        idx_t bits = 1 + floor_log2_uint64(k);
        seq_t shift = pow2_u(bits);
        res = res * shift + k;
    }

    return res;
}
