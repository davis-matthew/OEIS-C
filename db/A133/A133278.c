/*
 * A133278  Triangle read by rows, with n-th row the smallest non-constant
 *          n-term arithmetic progression of primes beginning with prime(n).
 * https://oeis.org/A133278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A133278: The a‑th term is the j‑th element of the r‑th row of the
   triangle where the r‑th row is the smallest non‑constant r‑term
   arithmetic progression of primes beginning with prime(r).
   The value is computed by brute‑force searching the minimal common
   difference. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 0;
    idx_t tri = 0;
    while (tri < n) {
        ++r;
        tri = r * (r + 1) / 2;
    }

    /* Position of n within the row */
    idx_t prev_tri = r * (r - 1) / 2;
    idx_t j = n - prev_tri;          /* 1‑based position in the row */

    /* Starting prime of this row */
    seq_t base = (seq_t)get_prime(r);

    /* Find the minimal difference d > 0 */
    seq_t d = 1;
    while (true) {
        bool ok = true;
        for (idx_t k = 0; k < r; ++k) {
            seq_t val = base + d * (seq_t)k;
            if (!is_prime_small(val)) {
                ok = false;
                break;
            }
        }
        if (ok) break;
        d = d + 1;
    }

    /* Return the j‑th term of the progression */
    return base + d * (seq_t)(j - 1);
}
