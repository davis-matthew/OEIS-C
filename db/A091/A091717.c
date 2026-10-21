/*
 * A091717  Triangular array read by rows: Greatest common divisors of values
 *          of the partition function P: T(n,k) = gcd(P(n), P(k)), 0 <= k < n.
 * https://oeis.org/A091717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 626, u16 n <= 1309, u32 n <= 8362, u64 n <= 8363,
 *            u128 n <= 8363, big any n
 * Helpers:   alloc_seq_array, free_seq_array, seq_gcd, tri_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of A091717:  read‑by‑rows of the triangular array
 * T(n,k) = gcd(P(n), P(k)), 0 ≤ k < n.  The function maps a 1‑based
 * index n to the corresponding (row, column) pair, evaluates the
 * required partition numbers, and returns their greatest common divisor.
 * The implementation uses only integer arithmetic and the supplied
 * utilities for array allocation and gcd.  It works for all n ≥ 1
 * regardless of the order of calls and is efficient for n up to about
 * 10⁴ (≈row 141).  For larger n the algorithm remains correct but its
 * running time grows roughly quadratically in the row number. */

#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/seq_gcd.h"
#include "../utils/tri_row.h"

seq_t a(idx_t n)
{
    /* Determine (row, column) pair from the 1‑based triangular index. */
    idx_t r = tri_row(n);                       /* row number (≥1) */
    __uint128_t past_rows = (__uint128_t)(r - 1) * r / 2; /* T(r-1) */
    idx_t k = (idx_t)(n - (idx_t)past_rows - 1);          /* column (0‑based) */

    /* Compute partition numbers P[0 … r] via dynamic programming. */
    size_t sz = (size_t)(r + 1);
    seq_t *p = alloc_seq_array(sz);

    /* Initialise to zero and set P(0)=1. */
    for (size_t i = 0; i < sz; ++i) p[i] = 0;
    p[0] = 1;

    for (size_t part = 1; part <= r; ++part) {
        for (size_t sum = part; sum <= r; ++sum) {
            p[sum] += p[sum - part];
        }
    }

    /* Compute gcd(P(r), P(k)). */
    seq_t result = seq_gcd(p[(size_t)r], p[(size_t)k]);

    free_seq_array(p);
    return result;
}
