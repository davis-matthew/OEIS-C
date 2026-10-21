/*
 * A224895  Let p = prime(n). Smallest odd number m > p such that m + p is
 *          semiprime.
 * https://oeis.org/A224895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7; for n > 1, a(n) = 2*next_prime(p(n)) – p(n), where
 *            p(n) is the n-th prime
 * Range:     u8 n <= 52, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1) {
        return (seq_t)7;
    }
    /* n ≥ 2: p is odd.  Find the next prime q > p. */
    uint32_t p32 = get_prime(n);          /* p fits in uint32_t for all typical ranges */
    uint64_t p  = (uint64_t)p32;          /* promote to 64‑bit for safety */
    uint64_t q  = next_prime(p);          /* q > p, odd */
    /* Compute 2*q – p in the result type seq_t. */
    seq_t twop = (seq_t)2 * (seq_t)q;
    seq_t result = (seq_t)p;
    result = twop - result;
    return result;
}
