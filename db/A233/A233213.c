/*
 * A233213  Number of n X 4 0..5 arrays with no element x(i,j) adjacent to
 *          value 5-x(i,j) horizontally or antidiagonally, top left element
 *          zero, and 1 appearing before 2 3 and 4, and 2 appearing before 3
 *          in row major order.
 * https://oeis.org/A233213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2205 + 486*49^n + 125*441^n) / 3528
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For seq_t of size ≤ 8 bytes (e.g. uint64_t) the computation is done in
   unsigned __int128 to avoid overflow. For larger seq_t (arbitrary‑precision
   integer classes) the calculation is performed directly with seq_t. */

#include <stdint.h>            /* for uint64_t   */

/* Integer exponentiation for arbitrary precision seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Integer exponentiation in unsigned __int128 (works for base ≤ 10^3, exp ≤ 10^4) */
static unsigned __int128 pow_uint128(unsigned base, idx_t exp)
{
    unsigned __int128 result = 1;
    unsigned __int128 b = base;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= b;
        b *= b;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Use 128‑bit arithmetic for small seq_t types (≤ 8 bytes) */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow49 = pow_uint128(49, n);
        unsigned __int128 pow441 = pow_uint128(441, n);

        unsigned __int128 term = (unsigned __int128)2205
                               + (unsigned __int128)486 * pow49
                               + (unsigned __int128)125 * pow441;

        unsigned __int128 result = term / 3528;
        return (seq_t)result;          /* cast to seq_t (uint8_t, uint16_t, …) */
    }

    /* Arbitrary‑precision path: use seq_t operations directly */
    seq_t pow49 = pow_seq((seq_t)49, n);
    seq_t pow441 = pow_seq((seq_t)441, n);

    seq_t term = (seq_t)2205
               + (seq_t)486 * pow49
               + (seq_t)125 * pow441;

    return term / (seq_t)3528;
}
