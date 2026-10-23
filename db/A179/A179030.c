/*
 * A179030  Primes whose only prime digits are 7's.
 * https://oeis.org/A179030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 408, u32 n <= 409, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_allowed_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculation: search successive primes whose decimal digits contain only
   the prime digit 7 (and no 2,3,5) and at least one 7.  */
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/has_allowed_digits.h"

#define CACHE_MAX 20000

static seq_t cache[CACHE_MAX];
static size_t cache_len = 0;
static seq_t next_start = 2;

seq_t a(idx_t n)
{
    /* Fast return if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate terms until we have n of them */
    while (cache_len < (size_t)n) {
        seq_t curr = next_start;
        /* Search for the next valid prime */
        while (1) {
            if (has_allowed_digits(curr) && is_prime_big(curr)) {
                if (cache_len == CACHE_MAX) {
                    /* should not happen for n <= 10000 */
                    abort();
                }
                cache[cache_len++] = curr;
                next_start = curr + 1;
                break;
            }
            ++curr;
        }
    }
    return cache[n - 1];
}
