/*
 * A177860  Product of the quadratic residues of prime(n).
 * https://oeis.org/A177860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product of the quadratic residues of prime(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) returns the k‑th prime */
#include "../utils/seq_one.h"    /* seq_one(void) returns seq_t value 1 */

/* Return the product of the quadratic residues modulo the n‑th prime. */
seq_t a(idx_t n)
{
    /* Prime corresponding to the index n. */
    uint32_t p = get_prime(n);

    /* Start product at 1. */
    seq_t prod = seq_one();

    /* For p = 2 the only residue is 1, so skip the loop. */
    if (p <= 2)
        return prod;

    /* Number of distinct non‑zero quadratic residues modulo an odd prime p
       is (p-1)/2. */
    idx_t half = (idx_t)((p - 1) / 2);

    /* Multiply the residues i^2 mod p for i = 1 .. (p-1)/2. */
    for (idx_t i = 1; i <= half; ++i) {
        /* i^2 modulo p fits in 32 bits. */
        uint32_t r = (uint32_t)(((uint64_t)i * i) % p);
        /* For integer seq_t this uses ordinary multiplication;
           for arbitrary‑precision seq_t the class overloads operator *. */
        prod = prod * (seq_t)r;
    }

    return prod;
}
