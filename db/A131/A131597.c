/*
 * A131597  Bigomega of Pisano periods mod n, i.e., number of prime divisors
 *          (counted with multiplicity) of the period of Fibonacci residues
 *          mod n.
 * https://oeis.org/A131597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = bigomega(Pisano(n))
 * Range:     u8 n <= 49, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of prime factors (with multiplicity) of the Pisano period
   of Fibonacci numbers modulo n. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Pisano(1) = 1, bigomega(1) = 0 */
    if (n == 1)
        return (seq_t)0;

    seq_t mod   = (seq_t)n;   /* modulus for Fibonacci iteration */
    seq_t prev  = 0;
    seq_t curr  = 1;
    seq_t next;
    seq_t period = 0;

    /* Upper bound for the period is 6*n for n >= 1. */
    idx_t limit = 6 * n + 5;  /* small safety margin */

    for (idx_t i = 0; i < limit; ++i) {
        next = (prev + curr) % mod;
        prev = curr;
        curr = next;
        ++period;

        if (prev == 0 && curr == 1)
            break;
    }

    return bigomega((idx_t)period);
}
