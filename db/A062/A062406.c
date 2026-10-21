/*
 * A062406  Cardinality of the (ordered) list L_n defined inductively by: L_1
 *          = {2}; L_(n+1) = L_n - {p}, where p is the first member of L_n,
 *          from left to right, such that prime(n+1) can be appended to the
 *          end or beginning of p so that the neighboring digits are equal, if
 *          p exists; append prime(n+1) to the e...
 * https://oeis.org/A062406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   computed by simulating construction of L_n as defined in OEIS
 *            A062406; returns |L_n|
 * Range:     u8 n <= 745, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   first_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/first_digit.h"

#define PRIME_LIMIT 200000      /* > 104729 (10 000th prime) */
#define MAX_PRIMES  20000      /* enough to hold primes up to PRIME_LIMIT */

/* primes array and count, initialized lazily */
static idx_t primes[MAX_PRIMES];
static size_t prime_cnt = 0;
static bool primes_built = false;

/* Simple sieve to fill the primes array up to PRIME_LIMIT */
static void build_primes(void)
{
    if (primes_built) return;
    bool *is_prime = (bool*)malloc(PRIME_LIMIT + 1);
    for (idx_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_prime[i]) continue;
        if (prime_cnt < MAX_PRIMES) primes[prime_cnt++] = i;
        if (i * 2 > PRIME_LIMIT) continue;
        for (idx_t j = i * 2; j <= PRIME_LIMIT; j += i)
            is_prime[j] = false;
    }
    free(is_prime);
    primes_built = true;
}

/* Main function required by the judge */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    build_primes();

    /* L_n never exceeds n, array size >= 20000 is safe for n up to ~10000 */
    static idx_t L[20000];
    size_t Llen = 1;
    L[0] = 2;                 /* L_1 = {2} */

    /* next prime to be considered starts at index 1 (the prime after 2) */
    size_t prime_index = 1;

    for (idx_t step = 2; step <= n; ++step) {
        idx_t prime = primes[prime_index++];
        idx_t first_p = first_digit(prime);
        idx_t last_p  = prime % 10;

        bool removed = false;
        for (size_t i = 0; i < Llen; ++i) {
            idx_t p = L[i];
            idx_t first_pn = first_digit(p);
            idx_t last_pn  = p % 10;
            if (last_p == first_pn || last_pn == first_p) {
                /* remove element i */
                for (size_t j = i; j + 1 < Llen; ++j)
                    L[j] = L[j + 1];
                --Llen;
                removed = true;
                break;
            }
        }
        if (!removed) {
            L[Llen++] = prime;   /* append */
        }
    }
    return (seq_t)Llen;
}
