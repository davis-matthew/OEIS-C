/*
 * A054013  Chowla function of n read modulo n.
 * https://oeis.org/A054013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (σ(n) − 1) mod n where σ(n) is the sum of all divisors of
 *            n. This is equivalent to the Chowla function of n read modulo n
 * Range:     u8 n <= 48, u16 n <= 9032, u32 n <= 9032, u64 n <= 9032,
 *            u128 n <= 9032, big any n
 * Helpers:   sigma_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_seq.h"

/* Main sequence function: a(n) = (σ(n) − 1) mod n, offset 1. */
seq_t a(idx_t n)
{
    /* n is expected ≥ 1; behaviour for n == 0 is undefined,
       but we return 0 to avoid division by zero. */
    if (n == 0)
        return 0;

    seq_t sig   = sigma_seq(n);
    seq_t value = sig - 1;                 /* σ(n) − 1 */
    seq_t res   = value % (seq_t)n;        /* modulo n  (n ≡ 0 mod n) */
    return res;
}
