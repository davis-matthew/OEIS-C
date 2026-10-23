/*
 * A194467  Numbers m such that Sum_{k=1..m} (<c + k*r> - <k*r>) < 0, where
 *          r=sqrt(3) and c=sqrt(1/3), and < > denotes fractional part.
 * https://oeis.org/A194467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   i_sqrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A194467 – numbers m such that
   S(m) = Σ_{k=1..m} ( <c + k√3> – <k√3> ) < 0.
   Let c = 1/√3.
   For each k, let a_k = <k√3>.  
   Then the summand is c if a_k < 1−c, otherwise c−1.
   Define count_bad(m) = #{k≤m : a_k ≥ 1−c}.
   S(m) = m·c – count_bad(m).
   The sign of S(m) can be checked without floating point:
      S(m) < 0   ⇔   m² < 3·count_bad(m)² .
   The condition a_k ≥ 1−c is equivalent to
      (3k+1)² ≥ 3·(⌊√(3k²)⌋+1)² .
   The function below computes S(m) incrementally and returns the
   n‑th term of the sequence in O(n) time (n ≤ 10⁴). */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/i_sqrt_uint64.h"

/*
 * a(idx_t n)   –   returns the n-th term of A194467
 *
 * The function scans m=1,2,… until n terms satisfying
 * S(m)<0 have been found.  All computations are
 * performed with integer arithmetic; no floating point
 * values are used for comparisons.
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for index 0 */

    uint64_t count_terms = 0;           /* number of terms found so far */
    uint64_t count_bad   = 0;           /* current count_bad(m) */
    uint64_t m           = 0;           /* current m being examined */

    while (count_terms < n) {
        ++m;

        /* floor_sqrt(3*m*m) gives ⌊k√3⌋ for k=m */
        uint64_t floor_k = i_sqrt_uint64(3 * m * m);

        /* test the condition a_k ≥ 1−c
           (3m+1)² ≥ 3·(floor_k+1)²  =>  bad index */
        __uint128_t lhs = (__uint128_t)(3 * m + 1) * (3 * m + 1);
        __uint128_t rhs = (__uint128_t)3 * (floor_k + 1) * (floor_k + 1);
        if (lhs >= rhs) ++count_bad;

        /* check S(m) < 0  ⇔  m² < 3·count_bad(m)² */
        __uint128_t m2     = (__uint128_t)m * m;
        __uint128_t bad2   = (__uint128_t)3 * count_bad * count_bad;
        if (m2 < bad2) {
            ++count_terms;
            if (count_terms == n) return (seq_t)m;
        }
    }
    /* control never reaches here */
    return (seq_t)0;
}
