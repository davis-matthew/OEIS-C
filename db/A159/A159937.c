/*
 * A159937  Triangle read by rows, A054525 * A127478, as infinite lower
 *          triangular matrices.
 * https://oeis.org/A159937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(i,j) = φ(j) * φ(i/j) if j divides i, otherwise 0; The sequence
 *            a(n) is the linear indexing of this lower‑triangular table (row
 *            r, column j, 1 ≤ j ≤ r)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: compute the smallest integer r such that r*(r+1)/2 >= n */
static idx_t row_of_n(idx_t n) {
    /* Solve r^2 + r - 2n >= 0 */
    idx_t lo = 0, hi = 1;
    while (hi * (hi + 1) / 2 < n) hi <<= 1;          /* exponential search */
    while (lo + 1 < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid * (mid + 1) / 2 < n) lo = mid; else hi = mid;
    }
    return hi;  /* row number (1‑based) */
}

/* Simple sieve for φ(1..max).  All arithmetic uses seq_t, which may be
   64‑bit or arbitrary precision; the intermediate integers fit in idx_t. */
static void compute_phi(idx_t max, seq_t *phi) {
    /* Allocate and initialize */
    for (idx_t i = 0; i <= max; ++i) phi[i] = (seq_t)i;
    phi[0] = 0;      /* unused */
    if (max >= 1) phi[1] = 1;

    /* Bitmask sieve of primes */
    uint8_t *is_prime = (uint8_t*)calloc(max + 1, sizeof(uint8_t));
    for (idx_t i = 2; i <= max; ++i) is_prime[i] = 1;

    for (idx_t i = 2; i <= max; ++i) {
        if (!is_prime[i]) continue;          /* i is composite */
        for (idx_t j = i; j <= max; j += i) {
            is_prime[j] = 0;
            phi[j] -= phi[j] / (seq_t)i;
        }
    }
    free(is_prime);
}

/* Main function requested by the build system */
seq_t a(idx_t n) {
    /* Determine the triangle row and column for the linear index n */
    idx_t r = row_of_n(n);              /* row number, 1‑based */
    idx_t offset = r * (r - 1) / 2;      /* number of entries before row r */
    idx_t c = n - offset;               /* column inside the row (1‑based) */

    /* Allocate φ up to row r (since we also need φ(r/c) ≤ φ(r)) */
    seq_t *phi = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    compute_phi(r, phi);

    /* Compute the value according to the rule described above */
    seq_t val = 0;
    if (c <= r && r % c == 0) {
        idx_t q = r / c;          /* integer quotient, ≤ r */
        val = phi[c] * phi[q];
    }

    free(phi);
    return val;
}
