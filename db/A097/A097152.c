/*
 * A097152  Smallest prime p greater than n for which n is a quadratic residue
 *          mod p.
 * https://oeis.org/A097152
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime p > n such that n is a quadratic residue
 *            modulo p
 * Range:     u8 n <= 205, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 86 terms through n = 9744 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* value below offset is irrelevant */

    uint64_t cand = n;     /* start just below the first possible candidate */
    for (;;)
    {
        cand = next_prime(cand);           /* smallest prime > current cand */
        if (jacobi_symbol((uint64_t)n, cand) == 1)
            return (seq_t)cand;            /* found the required prime */
    }
}
