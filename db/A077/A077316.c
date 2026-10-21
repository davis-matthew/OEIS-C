/*
 * A077316  Triangle read by rows: T(n,k) is the k-th prime = 1 (mod n).
 * https://oeis.org/A077316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = triangle read by rows where T(n,k) is the k‑th prime ≡ 1
 *            (mod n)
 * Range:     u8 n <= 44, u16 n <= 3482, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: determine the row r and position k, then find the k‑th prime of that congruence class. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Find the row r such that 1 + 2 + … + r ≥ n */
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* Number of elements before this row (computed in seq_t to avoid overflow) */
    seq_t before_seq = ((seq_t)(r - 1) * (seq_t)r) / 2;
    idx_t before = (idx_t)before_seq;
    idx_t k = n - before;                 /* position inside the row (1‑based) */

    if (r == 1) {                         /* row 1: all primes */
        if (k == 1) return (seq_t)2;      /* first prime */
        idx_t count = 1;                  /* prime 2 already counted */
        seq_t candidate = 3;
        while (count < k) {
            if (isPrimeGeneric(candidate)) {
                ++count;
                if (count == k) return candidate;
            }
            candidate += 2;               /* skip even numbers */
        }
    } else {
        seq_t candidate = (seq_t)1 + (seq_t)r;   /* first candidate > 1 with the desired congruence */
        idx_t count = 0;
        while (count < k) {
            if (isPrimeGeneric(candidate)) {
                ++count;
                if (count == k) return candidate;
            }
            candidate += (seq_t)r;
        }
    }
    /* Should never be reached, but return 0 to satisfy the compiler. */
    return (seq_t)0;
}
