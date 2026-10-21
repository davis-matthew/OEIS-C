/*
 * A240669  Number of the first odious exponents (A000069) in the prime power
 *          factorization of (2n)!.
 * https://oeis.org/A240669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 484, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A240669 – Number of the first odious exponents in the
    prime‑power factorization of (2n)!.
    Formula:  a(n) = number of initial exponents e_i (in increasing
    prime order) of (2n)! that are odious (have an odd popcount),
    stopping at the first non‑odious exponent. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

static inline bool is_odious(seq_t x)
{
    bool odd = false;
    while (x) {
        x &= x - 1;        // clear lowest set bit
        odd = !odd;        // toggle parity
    }
    return odd;
}

seq_t a(idx_t n)
{
    /* 2*n as idx_t */
    idx_t two_n = n + n;

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(two_n, &primes, &prime_cnt);

    seq_t count_odious = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t exp = 0;
        idx_t t = two_n;
        while (t) {
            t /= p;
            exp += (seq_t) t;          /* Legendre's formula */
        }
        if (is_odious(exp))
            ++count_odious;
        else
            break;                      /* stop at first non‑odious exponent */
    }

    free(primes);   /* cleanup */
    return count_odious;
}
