/*
 * A235481  Primes whose base-4 representation is also the base-9
 *          representation of a prime.
 * https://oeis.org/A235481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   sequence of primes whose base‑4 representation is also the *
 *            base‑9 representation of a prime. a(1)=2, a(2)=3, a(3)=29, ... *
 *            This implementation keeps a static cache of discovered terms.
 *            For * each new candidate prime p it computes the integer q
 *            obtained by * interpreting the base‑4 digits of p...
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 6496, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, base4_to_base9, bitlen_seq_t, is_prime,
 *            is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/base4_to_base9.h"

/* Cache storage */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Extend the cache until it contains at least n elements. */
static void generate_terms(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    static uint64_t last_checked = 2;  /* next integer to examine */

    for (;;) {
        uint64_t p = last_checked++;

        /* Skip non‑primes quickly */
        if (!is_prime((idx_t)p))
            continue;

        seq_t q = base4_to_base9(p);

        bool qprime;
        if (bitlen_seq_t(q) <= 64)
            qprime = is_prime((idx_t)q);
        else
            qprime = is_prime_big(q);

        if (qprime)
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p);

        if (cache_len >= (size_t)n)
            break;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n<1, harmless return */
        return 0;

    generate_terms(n);
    return cache[n - 1];
}
