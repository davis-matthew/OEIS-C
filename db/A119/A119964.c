/*
 * A119964  Numerator of the n-th Artin product.
 * https://oeis.org/A119964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = Numerator[ Product[ 1 - 1/(Prime[k]*(Prime[k]-1)),
 *            {k,1,n}]]
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

/* Computes the n-th term of OEIS sequence A119964.
   n >= 1. The function may be called in any order. */
seq_t a(idx_t n)
{
    seq_t num = 1;
    seq_t den = 1;

    for (idx_t k = 1; k <= n; ++k) {
        uint32_t p = get_prime(k);          /* k‑th prime, 32‑bit */
        seq_t pseq    = (seq_t)p;
        seq_t num_fact = pseq * pseq - pseq - (seq_t)1;  /* p^2 - p - 1 */
        seq_t den_fact = pseq * (pseq - (seq_t)1);        /* p * (p-1) */

        /* Reduce cross‑mix factors to keep numbers small */
        seq_t g1 = gcd(num_fact, den);
        num_fact /= g1;
        den      /= g1;

        seq_t g2 = gcd(num, den_fact);
        num     /= g2;
        den_fact /= g2;

        /* Multiply the remaining coprime parts */
        num *= num_fact;
        den *= den_fact;
    }

    return num;
}
