/*
 * A037128  a(n) = nextprime(3^n) - nextprime(2^n).
 * https://oeis.org/A037128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nextprime(3^n) - nextprime(2^n)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   is_prime_big, next_prime, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

/* Helper that returns the smallest prime > v */
static seq_t nextprime_seq(seq_t v)
{
    /* If v fits in 64‑bit, use the fast C routine */
    seq_t max_u64 = (seq_t)(~(uint64_t)0);
    if (v <= max_u64) {
        return (seq_t)next_prime((uint64_t)v);
    }
    /* Otherwise scan with the arbitrary‑precision primality test */
    seq_t cand = v;
    ++cand;
    while (!is_prime_big(cand)) {
        ++cand;
    }
    return cand;
}

seq_t a(idx_t n)
{
    /* Compute 2^n and 3^n as seq_t */
    seq_t two   = pow2(n);
    seq_t three = pow3(n);

    /* Next primes after these powers */
    seq_t p2 = nextprime_seq(two);
    seq_t p3 = nextprime_seq(three);

    /* Return the difference */
    return p3 - p2;
}
