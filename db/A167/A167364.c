/*
 * A167364  Triangle read by rows, A047999 * A010060 (diagonalized); as
 *          infinite lower triangular matrices.
 * https://oeis.org/A167364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A167364.
   a(n) is read from the triangle T(i,j) = (j & i == j) && (popcount(j) is odd)
   where i is the row number (starting at 1), j is the column number (1..i).
   The linear index n is obtained by enumerating rows i=1,2,... and
   columns j=1..i. */
#include <stdint.h>
#include <stdbool.h>

/* Helper: popcount for 64‑bit values.  Uses the compiler builtin if
   available, otherwise a loop. */
static unsigned int popcount_u64(uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
#endif
}

/* seq_t and idx_t are defined by the build system */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety, though n>=1 per spec */

    /* Determine the row i such that i*(i+1)/2 >= n. */
    idx_t i = 1;
    idx_t sum = 0;
    for (;; ++i) {
        sum += i;
        if (sum >= n) break;
    }

    /* j = column index in the row (1-based).  sum - i gives the
       cumulative total of previous rows. */
    idx_t j = n - (sum - i);

    /* Compute Thue–Morse bit: 1 if popcount(j) is odd. */
    unsigned int tm_bit = popcount_u64(j) & 1u;

    /* Compute binomial oddness: C(i, j) odd iff (j & i) == j. */
    unsigned int binom_bit = ((j & i) == j) ? 1u : 0u;

    return (seq_t)(tm_bit & binom_bit);
}
