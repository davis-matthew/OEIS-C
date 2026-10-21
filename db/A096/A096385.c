/*
 * A096385  a(n) = smallest prime p with p^n mod n = 1.
 * https://oeis.org/A096385
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that p^n mod n = 1 (inferred via
 *            brute‑force search)
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;                 /* undefined for n < 2, but harmless */

    seq_t mod = (seq_t)n;                /* modulus for remainders */
    seq_t p = 2;                         /* start from the first prime */

    for (;; ++p) {
        if (is_prime((idx_t)p) && mod_pow(p, (uint64_t)n, mod) == (seq_t)1)
            return p;
    }
}
