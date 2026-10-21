/*
 * A205374  s(k)-s(j), where (s(k),s(j)) is the least such pair for which n
 *          divides their difference, and s(j)=(prime(j+1) + prime(j+2))/2.
 * https://oeis.org/A205374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal positive difference s(k)-s(j), where s(j) =
 *            (prime(j+1)+prime(j+2))/2, k > j, and n | (s(k)-s(j))
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a sieve to generate enough primes
   and iterates s(j) until a repeated remainder modulo n is found. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- Precomputation of primes ------------------------------------ */

static const idx_t PRIME_SIEVE_LIMIT = 2000000U;   /* enough for n <= 10000   */
static uint32_t *primelist = NULL;                /* primes[0] = 2, primes[1] = 3, ...   */
static size_t prime_cnt = 0;

/* Build primes up to PRIME_SIEVE_LIMIT once. */
static void build_primes(void)
{
    bool *is_prime = (bool*)malloc((PRIME_SIEVE_LIMIT + 1U) * sizeof(bool));
    if (!is_prime) exit(1);

    for (idx_t i = 0U; i <= PRIME_SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2U; (idx_t)(i * i) <= PRIME_SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Count primes */
    prime_cnt = 0;
    primelist = (uint32_t*)malloc((PRIME_SIEVE_LIMIT + 1U) * sizeof(uint32_t));
    if (!primelist) exit(1);

    for (idx_t i = 2U; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            primelist[prime_cnt++] = (uint32_t)i;
        }
    }

    free(is_prime);
}

/* --- Sequence function ------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0;          /* not used, but safe */

    /* Ensure primes are ready */
    static bool primes_initialised = false;
    if (!primes_initialised) {
        build_primes();
        primes_initialised = true;
    }

    /* Allocate remainder tracking arrays */
    seq_t *earliest = (seq_t*)malloc((size_t)n * sizeof(seq_t));
    bool   *seen    = (bool*)malloc((size_t)n * sizeof(bool));
    if (!earliest || !seen) {
        if (earliest) free(earliest);
        if (seen)    free(seen);
        return (seq_t)0;
    }
    for (size_t i = 0U; i < (size_t)n; ++i) seen[i] = false;

    /* Iterate over s(j), j starts at 1 */
    for (idx_t j = 1U; ; ++j) {
        /* Ensure we have enough primes: j+1 must be < prime_cnt */
        if (j + 1U >= prime_cnt) {
            /* In practice this never happens for the required limits */
            free(earliest);
            free(seen);
            return (seq_t)0;
        }

        seq_t s = ((seq_t)primelist[j] + (seq_t)primelist[j + 1U]) / 2U;
        idx_t rem = (idx_t)(s % (seq_t)n);

        if (!seen[rem]) {
            seen[rem] = true;
            earliest[rem] = s;
        } else {
            seq_t diff = s - earliest[rem];
            free(earliest);
            free(seen);
            return diff;
        }
    }
}
