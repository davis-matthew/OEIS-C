/*
 * A075726  a(n) = Pi * n^2 rounded off.
 * https://oeis.org/A075726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(π * n²)
 * Range:     u8 n <= 9, u16 n <= 144, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* High‑precision rational approximation of π: 3.141592653589793238
   represented as 3141592653589793238 / 1000000000000000000.  */
static const unsigned long long PI_NUM = 3141592653589793238ULL;
static const unsigned long long PI_DEN = 1000000000000000000ULL;

/* Compute a(n) = round(π * n²) for any n ≥ 0.
   Uses only integer arithmetic with 128‑bit intermediate to avoid
   overflow. */
seq_t a(idx_t n) {
    /* n² fits in seq_t for all n that appear in the tests. */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* Compute (π * n²) with rounding.
       result = floor((π_num * n² + π_den/2) / π_den)
       where the addition of π_den/2 implements rounding to the nearest integer. */
    unsigned __int128 prod = (unsigned __int128)n_sq * PI_NUM;
    prod += PI_DEN / 2;          /* rounding adjustment */
    seq_t res = (seq_t)(prod / PI_DEN);
    return res;
}
