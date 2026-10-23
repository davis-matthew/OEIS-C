/*
 * A134798  Concatenation of next n odd isolated primes.
 * https://oeis.org/A134798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the concatenation of the next n odd isolated primes
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   digit_len, is_prime, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Isolated primes are odd primes p such that neither p-2 nor p+2 is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/pow10.h"     /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h" /* int digit_len(seq_t x); */

/* --------------------------------------------------------------------- */
/* Storage for isolated odd primes                                      */
static uint64_t *isolated_primes = NULL;   /* dynamic array of primes */
static size_t isolated_cap   = 0;          /* current capacity (bytes) */
static size_t isolated_len   = 0;          /* number of primes stored */

/* next odd candidate to test for primality (starts at 3) */
static uint64_t next_candidate = 3;

/* --------------------------------------------------------------------- */
/* Ensure the array contains at least 'need' isolated primes.          */
static void ensure_isolated(size_t need)
{
    if (isolated_primes == NULL) {
        isolated_cap = 1024;
        isolated_primes = (uint64_t *)malloc(isolated_cap * sizeof(uint64_t));
        if (!isolated_primes) exit(1);
    }

    while (isolated_len < need) {
        if (is_prime((idx_t)next_candidate)) {
            bool iso = !is_prime((idx_t)(next_candidate - 2)) &&
                       !is_prime((idx_t)(next_candidate + 2));
            if (iso) {
                if (isolated_len == isolated_cap) {
                    isolated_cap <<= 1;                /* double capacity */
                    uint64_t *tmp = (uint64_t *)
                        realloc(isolated_primes,
                                isolated_cap * sizeof(uint64_t));
                    if (!tmp) exit(1);
                    isolated_primes = tmp;
                }
                isolated_primes[isolated_len++] = next_candidate;
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined below offset */

    /* Number of isolated primes required up to and including this term */
    idx_t total_needed = n * ((idx_t)n + 1) / 2;
    ensure_isolated((size_t)total_needed);

    /* Zero‑based index of the first prime for this term */
    idx_t first_index = (n - 1) * n / 2;

    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t p = (seq_t)isolated_primes[(size_t)first_index + (size_t)i];
        int d = digit_len(p);
        seq_t mult = pow10((idx_t)d);
        result = seq_mul(result, mult);
        result = result + p;
    }
    return result;
}
