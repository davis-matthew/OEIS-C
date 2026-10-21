/*
 * A068887  Primes with property that digits alternate in parity individually
 *          as well as in concatenation with previous terms.
 * https://oeis.org/A068887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   digits_alternate, is_prime_big, lsd_parity, msd_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A068887 – primes whose decimal digits alternate in parity and whose
 * most–significant digit’s parity differs from the least–significant digit of
 * the preceding term.
 *
 * Each term is a prime p such that
 *   (1) digits(p) alternate even/odd,
 *   (2) if a(n‑1) exists then parity( msd(p) ) != parity( lsd( a(n‑1) ) ).
 *
 * Implementation: incremental generation with memoization.
 *   • is_prime_big(seq_t) is used for primality testing.
 *   • digits_alternate() checks intra‑prime parity alternation.
 *   • msd_parity() and lsd_parity() obtain parities of extremal digits.
 *   • the sequence is stored in a static dynamic array, expanded only as
 *     needed.  Calls with arbitrary n are answered from cache, so the
 *     ordering of calls is irrelevant.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/digits_alternate.h"
#include "../utils/lsd_parity.h"
#include "../utils/msd_parity.h"

/* Return the n‑th term of sequence A068887.  N is 1‑based. */
seq_t a(idx_t n)
{
    /* Cache of already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Expand cache until it contains n terms. */
    while (cache_len < n) {
        seq_t candidate;

        /* Start with the next integer after the last term. */
        if (cache_len == 0) {
            candidate = 2;
        } else {
            candidate = cache[cache_len - 1] + 1;
        }

        /* Search for the next admissible prime. */
        while (true) {
            /* Skip even numbers after the first prime. */
            if (candidate != 2 && (candidate & 1) == 0) {
                candidate = candidate + 1;
                continue;
            }

            /* 1) Must be prime. */
            if (!is_prime_big(candidate)) {
                candidate = candidate + 2;
                continue;
            }

            /* 2) Digits must alternate in parity. */
            if (!digits_alternate(candidate)) {
                candidate = candidate + 2;
                continue;
            }

            /* 3) Boundary parity must alternate with the previous term. */
            if (cache_len > 0 &&
                lsd_parity(cache[cache_len - 1]) == msd_parity(candidate)) {
                candidate = candidate + 2;
                continue;
            }

            /* Found a valid term. Store it. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
            break;      /* exit inner search loop */
        }
    }

    /* Convert the 1‑based index to 0‑based array index. */
    return cache[n - 1];
}
