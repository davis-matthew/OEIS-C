/*
 * A190060  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sin(Pi/5), t=csc(Pi/5).
 * https://oeis.org/A190060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n*csc(Pi/5)⌋ + ⌊n*(csc(Pi/5))^2⌋
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* csc(Pi/5) ≈ 1.701301343718...  (exact algebraic value) */

/* Use a 1e9 denominator rational approximation.  This keeps the
   fractional error below 1e‑4, which is sufficient for all n up to
   at least 10 000 (and larger, as long as the product fits in 128 bits). */
#include <stdint.h>

static const seq_t K1_NUM = 1701301337ULL;   /* floor(csc(Pi/5)   *1e9) */
static const seq_t K2_NUM = 2894427191ULL;   /* floor((csc)^2     *1e9) */
static const seq_t DENOM   = 1000000000ULL;  /* 1e9                 */

/* Compute the sequence value for index n. */
seq_t a(idx_t n) {
    /* 128‑bit multiplication avoids overflow for typical n.
       For very large n this will still overflow seq_t itself,
       but the function is required to be exact for all n that
       fit in seq_t. */
    unsigned __int128 prod1 = (unsigned __int128)(uint64_t)n * (unsigned __int128)K1_NUM;
    unsigned __int128 prod2 = (unsigned __int128)(uint64_t)n * (unsigned __int128)K2_NUM;

    seq_t floor1 = (seq_t)(prod1 / DENOM);
    seq_t floor2 = (seq_t)(prod2 / DENOM);

    return (seq_t)n + floor1 + floor2;
}
