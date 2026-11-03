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
 * Validated: standard, pass, 86 terms through n = 838 (2026-11-01)
 * Helpers:   is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;                 /* undefined for n < 2, but harmless */

    seq_t mod = (seq_t)n;                /* modulus for remainders */
    seq_t p = 2;                         /* start from the first prime */

    for (;; ++p) {
        if (is_prime((idx_t)p) && powmod(p, (uint64_t)n, mod) == (seq_t)1)
            return p;
    }
}
