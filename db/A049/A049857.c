/*
 * A049857  Concatenate prevprime(n), n, and nextprime(n).
 * https://oeis.org/A049857
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate(prevprime(n), n, nextprime(n))
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 32 (2026-11-01)
 * Helpers:   is_prime, next_prime, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = ⟨prevprime(n)⟩‖⟨n⟩‖⟨nextprime(n)⟩ in decimal representation. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"
#include "../utils/num_digits.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n >= 3 by OEIS offset */
    /* Find previous and next primes */
    idx_t prev = prev_prime(n);
    uint64_t nxt_ul = next_prime((uint64_t)n); /* next_prime expects uint64_t */

    /* Convert to seq_t for arithmetic */
    seq_t prev_s = (seq_t)prev;
    seq_t nxt_s  = (seq_t)nxt_ul;
    seq_t n_s    = (seq_t)n;

    /* Compute decimal lengths */
    int d_n     = num_digits(n_s);
    int d_next  = num_digits(nxt_s);

    /* Compute powers of ten for concatenation */
    seq_t pow_len = pow10((idx_t)(d_n + d_next));          /* 10^(d(n)+d(next))      */
    seq_t pow_next = pow10((idx_t)d_next);                /* 10^(d(next))            */

    /* Concatenate prev + n + next */
    seq_t mid = n_s * pow_next + nxt_s;                   /* n * 10^d(next) + next  */
    seq_t result = prev_s * pow_len + mid;                /* prev * 10^len(mid) + mid */

    return result;
}
