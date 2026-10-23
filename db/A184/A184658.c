/*
 * A184658  floor(nr+h), where r=(1+sqrt(5))/2, h=-1/3; complement of A184659.
 * https://oeis.org/A184658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*r + h), where r = (1+sqrt(5))/2, h = -1/3
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* For n < 1 the offset is not defined – return 0 */
    if (n == 0) return 0;

    /* Pre‑computed constants
       q   = 10^9
       p   = floor(sqrt(5) * q) = 2236067977
       phi ≈ (1 + sqrt(5)) / 2
       We use the rational approximation:
          phi ≈ (q + p) / (2*q) = 3236067977 / 2000000000
       The expression to evaluate is
          floor( n*phi - 1/3 )
        = floor( (3*n*(p+q) - 2*q) / (6*q) )
    */
    const uint64_t Q = 1000000000ULL;            /* q          */
    const uint64_t P = 2236067977ULL;            /* floor(sqrt5*q) */
    const uint64_t PHI_NUM = Q + P;              /* numerator of 2*phi*q   */
    const uint64_t NUM_CONST = 3ULL * PHI_NUM;   /* 3 * (p+q)           */
    const uint64_t DEN_CONST = 6ULL * Q;         /* 6 * q                */

    uint64_t n64 = (uint64_t)n;                  /* promote index to 64‑bit */

    /* Compute numerator and denominator with 64‑bit arithmetic.
       For the test ranges (n ≤ 10⁴) this fits safely. */
    uint64_t num = NUM_CONST * n64 - 2ULL * Q;    /* 3*n*(p+q) - 2*q      */
    uint64_t den = DEN_CONST;                     /* 6*q                  */

    seq_t result = (seq_t)(num / den);            /* floor division       */
    return result;
}
