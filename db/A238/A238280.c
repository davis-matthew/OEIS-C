/*
 * A238280  Irregular triangle read by rows, T(n,k) = Sum_{i = 1..n} k mod i,
 *          k = 1..m where m = lcm(1..n).
 * https://oeis.org/A238280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Σ_{i=1..r} (k mod i), where r is the smallest integer
 *            with Σ_{j=1..r} lcm(1..j) ≥ n, and k = n – Σ_{j=1..r-1}
 *            lcm(1..j)
 * Range:     u8 n <= 6441, u16 n <= 6441, u32 n <= 6441, u64 n <= 6441,
 *            u128 n <= 6441, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic, respecting the
   variable-width types provided by the build (seq_t and idx_t). */

#include <stdint.h>
#include "../utils/gcd_u64.h"

/* Compute GCD using 64‑bit arithmetic to avoid overflow when
   lcm values exceed the width of seq_t. */

/* OEIS sequence A238280. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range calls */

    /* Find the row r containing the nth element. */
    uint64_t cum   = 0;             /* cumulative count of terms seen */
    uint64_t lcm   = 1;             /* lcm(1..r-1) before processing row r */
    idx_t r        = 1;

    while (1) {
        /* current row length = lcm(1..r) */
        if (r == 1)
            lcm = 1;
        else {
            uint64_t g = gcd_u64(lcm, (uint64_t)r);
            lcm = (lcm / g) * (uint64_t)r;   /* fits in 64 bits for needed r */
        }

        uint64_t next_cum = cum + lcm;
        if ((uint64_t)n <= next_cum)      /* nth element lies in this row */
            break;

        cum = next_cum;
        r++;
    }

    /* Position within the identified row (1‑based). */
    uint64_t k = (uint64_t)n - cum;
    seq_t sum   = 0;

    /* Compute Σ (k mod i) for i = 1..r. */
    for (idx_t i = 1; i <= r; ++i) {
        sum += (seq_t)(k % (uint64_t)i);
    }

    return sum;
}
