/*
 * A210185  Sum of distinct residues of all factorials mod prime(n).
 * https://oeis.org/A210185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct residues of m! (mod prime(n)), for all m
 *            ≥ 1. That is: let p = prime(n). Compute k! mod p for k = 1 …
 *            p−1, collect the distinct values (including 0), and return the
 *            arithmetic sum of those values
 * Range:     u8 n <= 10, u16 n <= 87, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.  For n = 0 we
       return 0 to keep the function defined. */
    if (n == 0)
        return (seq_t)0;

    /* Retrieve the n‑th prime.  For n ≤ 10000 the value fits in 32 bits. */
    uint32_t p = get_prime(n);
    if (p <= 1)
        return (seq_t)0;

    /* Boolean array marking which residues have already been seen. */
    uint8_t *seen = (uint8_t *)malloc((size_t)p);
    if (!seen)
        return (seq_t)0;   /* out of memory – unlikely for the supported limits */
    memset(seen, 0, (size_t)p);

    seq_t sum = (seq_t)0;
    uint64_t fact = 1;      /* factorial value modulo p */

    /* Compute k! mod p for k = 1 … p-1.  The residue 0 appears only
       for k ≥ p, so it is handled separately (adding 0 does nothing). */
    for (uint32_t k = 1; k < p; ++k) {
        fact = (fact * k) % p;
        uint32_t r = (uint32_t)fact;          /* 0 ≤ r < p */
        if (!seen[r]) {
            seen[r] = 1;
            sum += (seq_t)fact;
        }
    }

    /* Residue 0 occurs for all m ≥ p; adding it does not change the sum. */

    free(seen);
    return sum;
}
