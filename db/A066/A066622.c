/*
 * A066622  Concatenation of prime numbers in increasing order up to the n-th
 *          and then in decreasing order.
 * https://oeis.org/A066622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat(p₁p₂…pₙ , pₙ₋₁…p₁) where pᵢ is the i‑th prime
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

static uint64_t *prime_list = NULL;   /* primes up to current limit */
static size_t      prime_cnt  = 0;     /* number of primes stored */
static idx_t       prime_limit = 0;    /* upper bound used for sieve */

/*
 * Compute the n-th term of A066622.
 * The function automatically builds the required primes if needed
 * and concatenates them as described.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; return 0 for safety */

    /* Ensure we have enough primes for the requested n. */
    if ((size_t)n > prime_cnt) {
        /* Compute a generous upper bound for the n‑th prime. */
        idx_t new_limit = _prime_limit_for_idx(n);
        /* Free previous array if it exists. */
        if (prime_list)
            free(prime_list);
        /* Build all primes up to the new limit. */
        build_primes_upto(new_limit, &prime_list, &prime_cnt);
        prime_limit = new_limit;
    }

    seq_t result = 0;

    /* First part: increase 1→n. */
    for (size_t i = 0; i < (size_t)n; ++i) {
        uint64_t p   = prime_list[i];
        int      d   = digit_len((seq_t)p);
        result = result * pow10((idx_t)d) + (seq_t)p;
    }

    /* Second part: decrease (n‑1)→1. */
    for (idx_t i = n - 1; i > 0; --i) {
        uint64_t p   = prime_list[i - 1];
        int      d   = digit_len((seq_t)p);
        result = result * pow10((idx_t)d) + (seq_t)p;
    }

    return result;
}
