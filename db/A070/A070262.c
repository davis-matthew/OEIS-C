/*
 * A070262  5th diagonal of triangle defined in A051537.
 * https://oeis.org/A070262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n + 4, n) / gcd(n + 4, n) which simplifies to a(n) =
 *            n * (n + 4) / (gcd(n + 4, n))^2 for n ≥ 1 (offset 1)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute gcd of two unsigned 64‑bit integers. */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    idx_t n4 = n + 4;                     /* (n + 4)  */
    idx_t g  = gcd_idx(n, n4);            /* gcd(n, n+4)  */
    /* Use 128‑bit arithmetic for the multiplication to avoid overflow. */
    unsigned __int128 num = (unsigned __int128)n * (unsigned __int128)n4;   /* n*(n+4) */
    unsigned __int128 den = (unsigned __int128)g * (unsigned __int128)g;   /* g*g   */
    unsigned __int128 res128 = num / den;      /* exact division */
    return (seq_t)res128;                     /* cast to seq_t (truncation is acceptable in 64‑bit build) */
}
