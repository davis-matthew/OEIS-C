/*
 * A232505  Sum of odd quadratic residues of prime(n).
 * https://oeis.org/A232505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd quadratic residues of the n-th prime
 * Range:     u8 n <= 15, u16 n <= 130, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/*  Return the sum of all odd residues r (< prime(n)) such that
    r ≡ k² (mod prime(n)) for some integer k.  Handles the
    special case p = 2 separately, since the loop below
    would iterate zero times for that prime. */
seq_t a(idx_t n)
{
    uint64_t p = (uint64_t)get_prime(n);   /* the n-th prime */

    if (p == 2)
        return (seq_t)1;                    /* residues: {1} */

    seq_t sum = 0;
    uint64_t half = (p - 1) / 2;          /* number of distinct non‑zero residues */

    for (uint64_t k = 1; k <= half; ++k) {
        uint64_t r = (k * k) % p;          /* quadratic residue modulo p */
        if (r & 1)                         /* keep only odd residues */
            sum += (seq_t)r;
    }
    return sum;
}
