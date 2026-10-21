/*
 * A077197  a(n) = lambda(phi(n)) where lambda(n) (A002322) is the Carmichael
 *          lambda function which gives the smallest integer m such that k^m =
 *          1 mod n for all integers k relatively prime to n and phi(n) is
 *          A000010.
 * https://oeis.org/A077197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lambda(phi(n)) where lambda is the Carmichael lambda
 *            function
 * Range:     u8 n <= 561, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64, lambda_u64, phi_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/phi_idx.h"   /* seq_t phi_idx(idx_t n); */
#include "../utils/gcd_u64.h"
#include "../utils/lambda_u64.h"

/* -------------------------------------------------------------
   Internal helpers
   ------------------------------------------------------------- */

/* -------------------------------------------------------------
   Sequence function
   ------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* φ(n) is returned as seq_t by the utility function. */
    seq_t phi_val = phi_idx(n);

    /* λ(φ(n)) */
    uint64_t lambda_val = lambda_u64((uint64_t)phi_val);

    return (seq_t)lambda_val;
}
