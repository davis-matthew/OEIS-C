/*
 * A030635  Numbers with 17 divisors.
 * https://oeis.org/A030635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000040(n)^16 (nth prime raised to the 16th power)
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static uint64_t *primes   = NULL;  /* array of known primes          */
static size_t   primes_len = 0;    /* number of primes currently stored */
static size_t   primes_cap = 0;    /* allocated capacity for 'primes'   */

/* Ensure that the first 'n' primes are available in the static array.
   The routine grows the array as necessary and fills the missing primes
   by trial division using the primes already known. */
static void
ensure_primes(idx_t n)
{
    if (n <= primes_len)
        return;

    /* Allocate or enlarge the primes array */
    size_t needed = (size_t)n;
    if (primes_cap < needed) {
        size_t new_cap = primes_cap ? primes_cap : 128;
        while (new_cap < needed)
            new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp && needed > 0)
            abort();                      /* allocation failed */
        primes = tmp;
        primes_cap = new_cap;
    }

    /* Generate missing primes */
    uint64_t candidate = primes_len ? primes[primes_len - 1] + 1 : 2;
    while (primes_len < n) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p > candidate / p)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            primes[primes_len++] = candidate;
        ++candidate;
    }
}

/* OEIS sequence A030635: numbers with 17 divisors (16th powers of primes). */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n < 1, but 0 is safe */

    ensure_primes(n);
    seq_t prime = (seq_t)primes[n - 1];
    return big_pow(prime, (seq_t)16);
}
