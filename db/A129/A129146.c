/*
 * A129146  a(n) = n-th odd prime minus n-th odd composite number.
 * https://oeis.org/A129146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = A065091(n) – A071904(n) (odd prime → nth odd composite)
 * Range:     i8 n <= 75, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   ensure_array_seqt, ensure_array_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_array_seqt.h"
#include "../utils/ensure_array_uint32.h"

/* ------------- static tables ------------------------------------ */
static uint32_t *odd_primes      = NULL;   /* odd primes 3,5,7,...            */
static size_t   odd_primes_cnt   = 0;
static size_t   odd_primes_cap   = 0;

static seq_t *odd_composites     = NULL;   /* odd composites 9,15,21,...     */
static size_t   odd_composites_cnt = 0;
static size_t   odd_composites_cap = 0;

static uint64_t next_to_test = 3;          /* next odd number to test       */

/* ------------- expand data until we have at least n entries ----- */
static void ensure_limit(idx_t n)
{
    if (odd_primes_cnt >= n && odd_composites_cnt >= n) return;

    while (odd_primes_cnt < n || odd_composites_cnt < n) {
        uint64_t x = next_to_test;
        bool is_prime = true;

        for (size_t i = 0; i < odd_primes_cnt; ++i) {
            uint64_t p = odd_primes[i];
            if (p * p > x) break;
            if (x % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            ensure_array_uint32(&odd_primes, &odd_primes_cnt, &odd_primes_cap);
            odd_primes[odd_primes_cnt++] = (uint32_t)x;
        } else {
            ensure_array_seqt(&odd_composites, &odd_composites_cnt, &odd_composites_cap);
            odd_composites[odd_composites_cnt++] = (seq_t)x;
        }
        next_to_test += 2;
    }
}

/* ------------- public sequence function ------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence defined for n >= 1 */
    ensure_limit(n);

    seq_t prime_val = (seq_t)odd_primes[n - 1];
    seq_t comp_val  = odd_composites[n - 1];

    return prime_val - comp_val;
}
