/*
 * A135076  Primes appearing in A001370.
 * https://oeis.org/A135076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135076 (Primes appearing in A001370).
   a(n) = A007953(2^A076203(n)) = prime digit sum of 2^(e),
   where e is the n-th exponent for which the digit sum of 2^e is prime.
   The function generates the sequence on demand and stores the values.
*/
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PRIME_LIMIT 200000   /* Safe upper bound for digit sums up to e≈20000     */

/* Static state for incremental generation */
static size_t cache_len   = 0;   /* number of terms already produced            */
static size_t cache_cap   = 0;   /* allocated capacity of cache array            */
static seq_t *cache       = NULL;/* array holding the prime digit sums           */

static idx_t  curr_exp    = 0;   /* current exponent e of 2^e (starting with 0) */
static uint8_t *digits    = NULL;/* decimal digits of 2^e, least-significant first */
static size_t digits_len  = 0;   /* number of used digits                       */
static size_t digits_cap  = 0;   /* allocated capacity of digits array          */

static unsigned sum_digits = 0;  /* sum of decimal digits of 2^curr_exp         */

/* Prime sieve */
static bool *prime_sieve   = NULL;/* true if index is prime, false otherwise    */
static size_t sieve_limit  = 0;   /* filled up to this value                    */

/* Initialize the prime sieve up to PRIME_LIMIT */
static void init_sieve(void)
{
    if (prime_sieve) return;          /* already initialized */
    sieve_limit = PRIME_LIMIT;
    prime_sieve = (bool*)malloc((sieve_limit + 1) * sizeof(bool));
    if (!prime_sieve) return;
    memset(prime_sieve, true, sieve_limit + 1);
    prime_sieve[0] = prime_sieve[1] = false;
    for (size_t i = 2; i * i <= sieve_limit; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j <= sieve_limit; j += i)
                prime_sieve[j] = false;
        }
    }
}

/* Ensure the cache has capacity for at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    if (!cache_cap) cache_cap = 16;
    while (cache_cap < need) cache_cap <<= 1;
    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
}

/* Ensure digits array has capacity for 'need' digits. */
static void ensure_digits_cap(size_t need)
{
    if (digits_cap >= need) return;
    while (digits_cap < need) digits_cap <<= 1;
    digits = (uint8_t*)realloc(digits, digits_cap * sizeof(uint8_t));
}

/* Ensure the sieve covers values up to 'limit' (called only if needed). */
static void extend_sieve(size_t limit)
{
    if (limit <= sieve_limit) return;
    bool *new_sieve = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!new_sieve) return;
    memset(new_sieve, true, limit + 1);
    new_sieve[0] = new_sieve[1] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (new_sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                new_sieve[j] = false;
        }
    }
    free(prime_sieve);
    prime_sieve = new_sieve;
    sieve_limit = limit;
}

/* Main function as required by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n<1, return 0 */

    /* Initialise static structures on first call. */
    if (!prime_sieve) init_sieve();
    if (!digits) {
        digits_cap = 16;
        digits = (uint8_t*)malloc(digits_cap);
        digits[0] = 1;                  /* 2^0 = 1 */
        digits_len = 1;
        sum_digits = 1;
        curr_exp = 0;                   /* exponent 0 has been represented */
    }

    /* Generate terms until we have at least n terms. */
    while (cache_len < (size_t)n) {
        /* Multiply current decimal representation by 2. */
        unsigned carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            unsigned val = digits[i] * 2 + carry;
            unsigned new_d = val % 10;
            carry = val / 10;
            unsigned diff = new_d - digits[i];
            sum_digits += diff;          /* update digit sum incrementally   */
            digits[i] = new_d;
        }
        while (carry) {
            unsigned new_d = carry % 10;
            carry /= 10;
            if (digits_len == digits_cap)
                ensure_digits_cap(digits_cap * 2);
            digits[digits_len++] = new_d;
            sum_digits += new_d;
        }
        ++curr_exp;                      /* exponent increased by 1           */

        /* Check if the new digit sum is prime. */
        if (sum_digits <= sieve_limit && prime_sieve[sum_digits]) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len] = (seq_t)sum_digits;
            ++cache_len;
        }
    }

    return cache[(size_t)n - 1];
}
