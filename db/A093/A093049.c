/*
 * A093049  n-1 minus exponent of 2 in n, a(0) = 0.
 * https://oeis.org/A093049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 - v2(n), a(0) = 0 where v2(n) is the exponent of 2 in
 *            the prime factorisation of n (the number of trailing zeros in
 *            the binary representation)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute the value of the sequence at index n */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* defined special case */

    /* determine exponent of 2 in n */
#if defined(__GNUC__) || defined(__clang__)
    /* use built‑in for speed; only for 64‑bit n which is our idx_t type */
    unsigned v2 = __builtin_ctzll((unsigned long long)n);
#else
    /* portable fallback */
    unsigned v2 = 0;
    while ((n & 1ULL) == 0ULL) { n >>= 1; ++v2; }
#endif

    /* cast to seq_t and compute the formula */
    seq_t n_seq = (seq_t)n;
    return n_seq - (seq_t)1 - (seq_t)v2;
}
