/*
 * A167839  Five-times-isolated primes: primes p such that none of
 *          (p-10,p-8,p-6,p-4,p-2,p+2,p+4,p+6,p+8,p+10) are prime.
 * https://oeis.org/A167839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Five-times-isolated primes.
   The function enumerates primes p such that none of
   p±2, p±4, p±6, p±8, p±10 are prime.
   The algorithm precomputes a sieve up to a fixed limit
   and stores all such primes in an array.
   It then returns the (n-1)-th element of that array.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Compile‑time limit for the sieve.  10 million comfortably
   contains the first ten thousand isolated primes.           */
#define SIEVE_LIMIT 10000000UL

/* Static state:  sieve, isolated primes list.               */
static bool *is_prime = NULL;            /* sieve array             */
static size_t sieve_limit = SIEVE_LIMIT;

static seq_t *isolated = NULL;           /* list of isolated primes */
static size_t isolated_cnt = 0;          /* number already found    */
static size_t isolated_cap = 0;          /* allocated capacity      */
static bool initialized = false;

/* Initialize the sieve and pre‑compute the isolated primes. */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate sieve.  We use a uint8_t array for simplicity. */
    is_prime = (bool *)malloc((sieve_limit + 1) * sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);   /* abort on allocation failure */

    /* Initialise every entry to true, then set 0 and 1 to false. */
    memset(is_prime, 1, sieve_limit + 1);
    if (sieve_limit >= 0) is_prime[0] = false;
    if (sieve_limit >= 1) is_prime[1] = false;

    /* Classic Eratosthenes sieve. */
    for (size_t i = 2; i * i <= sieve_limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= sieve_limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Prepare array for isolated primes.  Start with a small capacity. */
    isolated_cap = 128;
    isolated = (seq_t *)malloc(isolated_cap * sizeof(seq_t));
    if (!isolated) exit(EXIT_FAILURE);

    /* Offsets to check around a candidate prime. */
    const int offsets[10] = {-10, -8, -6, -4, -2, 2, 4, 6, 8, 10};

    /* Scan all primes and collect the isolated ones. */
    for (size_t p = 2; p <= sieve_limit; ++p) {
        if (!is_prime[p]) continue;

        bool isolated_candidate = true;
        for (int k = 0; k < 10; ++k) {
            long long neigh = (long long)p + offsets[k];
            if (neigh >= 2 && neigh <= (long long)sieve_limit && is_prime[neigh]) {
                isolated_candidate = false;
                break;
            }
        }

        if (isolated_candidate) {
            if (isolated_cnt == isolated_cap) {
                isolated_cap *= 2;
                isolated = (seq_t *)realloc(isolated,
                                            isolated_cap * sizeof(seq_t));
                if (!isolated) exit(EXIT_FAILURE);
            }
            isolated[isolated_cnt++] = (seq_t)p;
        }
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0) return (seq_t)0;          /* undefined for n<1, just safe */

    /* Return the (n-1)-th isolated prime. */
    return isolated[n - 1];
}
