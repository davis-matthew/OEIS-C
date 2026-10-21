/*
 * A089657  Let g[n]=Floor[Prime[n]-n)/((n-PrimePi[n])], then a(n) =
 *          Floor[(Prime[n]+g[n]*PrimePi[n])/(1+g[n]).
 * https://oeis.org/A089657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor((Prime[n] + g[n]*PrimePi[n])/(1+g[n])) , where g[n]
 *            = floor((Prime[n] - n)/(n - PrimePi[n])). Prime[n] : the n-th
 *            prime, PrimePi[n] : π(n), the number of primes ≤ n
 * Range:     u8 n <= 42, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for primes and π values */
static bool   *sieve      = NULL;   /* sieve[i] == true ⇔ i is prime    */
static uint32_t *pi_arr   = NULL;   /* pi_arr[i] = π(i)                 */
static uint64_t *prime_list = NULL; /* list of primes, 0‑based          */
static size_t    prime_count = 0;    /* number of primes stored         */
static idx_t     sieve_limit = 0;    /* current maximum n processed     */

/* build sieve, π array and prime list up to at least max_n */
static void ensure_sieve(idx_t max_n)
{
    if (sieve_limit >= max_n)
        return;                 /* already computed up to required size */

    /* Choose a limit that covers both n and the n‑th prime.
       For n ≤ 10000 the 10000‑th prime is 104729, so 200000 is safe. */
    idx_t new_limit = (max_n < 200000) ? 200000 : max_n;

    /* allocate/reallocate arrays */
    sieve      = (bool*)      realloc(sieve,      (new_limit + 1) * sizeof(bool));
    pi_arr     = (uint32_t*)   realloc(pi_arr,     (new_limit + 1) * sizeof(uint32_t));
    prime_list = (uint64_t*)   realloc(prime_list,  20000  * sizeof(uint64_t)); /* dummy cap, will grow */

    /* initialise sieve */
    for (idx_t i = 0; i <= new_limit; ++i) sieve[i] = true;
    if (new_limit >= 0) { sieve[0] = false; sieve[1] = false; }

    /* classic Eratosthenes */
    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                sieve[j] = false;
        }
    }

    /* build π array */
    uint32_t count = 0;
    for (idx_t i = 0; i <= new_limit; ++i) {
        if (i >= 2 && sieve[i]) ++count;
        pi_arr[i] = count;
    }

    /* build prime list */
    size_t cap = 20000;
    size_t idx = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (sieve[i]) {
            if (idx >= cap) {
                cap *= 2;
                prime_list = (uint64_t*)realloc(prime_list, cap * sizeof(uint64_t));
            }
            prime_list[idx++] = (uint64_t)i;
        }
    }
    prime_count = idx;
    sieve_limit = new_limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* below offset: undefined, return 0 */

    ensure_sieve(n);                     /* make prime table ready */

    seq_t prime_n = (seq_t)prime_list[n - 1]; /* n-th prime, 1‑based index */
    seq_t pi_n    = (seq_t)pi_arr[n];           /* π(n) */

    seq_t numerator   = prime_n - (seq_t)n;
    seq_t denominator = (seq_t)n - pi_n;        /* > 0 for all n ≥ 1 */

    seq_t g = numerator / denominator;          /* floor division */

    seq_t top    = prime_n + g * pi_n;
    seq_t bottom = 1 + g;

    return top / bottom;                        /* floor division */
}
