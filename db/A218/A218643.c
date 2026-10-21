/*
 * A218643  Array, by antidiagonals, A(n,k) = next number that is the product
 *          of exactly k primes (not necessarily distinct) after 10*n.
 * https://oeis.org/A218643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   _floor_isqrt_u64, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A218643:
   a(n) is obtained by antidiagonal traversal of the array A(n,k),
   where A(n,k) is the smallest integer m > 10*n such that the number
   of prime factors of m (with multiplicity) equals k.
   The diagonal index s and offset pos are computed from n, and the
   value is found by testing successive integers for the required
   value of bigomega. */

#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n) {
    /* n is 1–based.  Compute the diagonal s (≥1) such that
       T(s-1) < n ≤ T(s), where T(s)=s*(s+1)/2.  */
    idx_t discr = 8 * n + 1;
    idx_t sqrt_d = _floor_isqrt_u64(discr);
    idx_t s = (sqrt_d - 1) / 2;          /* floor of the formula */
    if (s * (s + 1) / 2 < n)             /* adjust if needed    */
        s++;

    /* Determine position within the diagonal. */
    idx_t prev = s * (s - 1) / 2;        /* T(s-1)                 */
    idx_t pos = n - prev - 1;            /* 0‑based within diagonal */
    idx_t n_coord = pos;                 /* 'n' of A(n,k)          */
    idx_t k_coord = s - pos;             /* 'k' of A(n,k)          */

    /* Search for the smallest m > 10*n_coord with bigomega(m)==k_coord. */
    idx_t val = 10 * n_coord + 1;
    seq_t target = (seq_t)k_coord;
    while (1) {
        if (bigomega(val) == target)
            return (seq_t)val;
        val++;
    }
}
