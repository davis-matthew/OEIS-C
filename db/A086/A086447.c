/*
 * A086447  a(n) = the least k such that prime(n+1)+prime(n+2)+...+prime(n+k)
 *          is a multiple of prime(n).
 * https://oeis.org/A086447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least k such that prime(n+1)+...+prime(n+k) is a multiple
 *            of prime(n)
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 3375, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Global storage for primes and their cumulative sums
 * primes[0] = 2, primes[1] = 3, ...
 * cum[i] = sum of the first i primes (cum[0] = 0) */
static idx_t prime_cnt = 0;
static size_t prime_cap = 0;
static uint64_t *primes = NULL;
static size_t cum_cap = 0;
static seq_t *cum = NULL;

/* Ensure the prime and cumulative arrays have room for at least `need` primes */
static void grow_primes(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 1024;
    if (new_cap < need) new_cap = need;
    uint64_t *new_primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!new_primes) abort();
    primes = new_primes;
    prime_cap = new_cap;

    seq_t *new_cum = (seq_t *)realloc(cum, (new_cap + 1) * sizeof(seq_t));
    if (!new_cum) abort();
    cum = new_cum;
    cum_cap = new_cap + 1;
}

/* Generate the next prime and update the cumulative sum array */
static void add_next_prime(void)
{
    /* Find the next candidate prime */
    uint64_t cand = prime_cnt ? primes[prime_cnt - 1] + 1 : 2;
    for (;; ++cand) {
        int is_prime = 1;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            /* Ensure arrays large enough */
            grow_primes(prime_cnt + 1);

            /* Store new prime */
            primes[prime_cnt] = cand;
            /* Update cumulative sum: cum[prime_cnt+1] = cum[prime_cnt] + cand */
            cum[prime_cnt + 1] = cum[prime_cnt] + (seq_t)cand;
            ++prime_cnt;
            break;
        }
    }
}

/* Ensure that there are at least `need` primes stored */
static void ensure_primes(idx_t need)
{
    while (prime_cnt < need) add_next_prime();
}

/* Main function: a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 in OEIS data */
    ensure_primes(n + 1);                 /* need prime(n) and at least one more */

    seq_t p = (seq_t)primes[n - 1];       /* prime(n) */
    idx_t k = 1;
    for (;;) {
        idx_t end = n + k;                /* need primes up to index n+k-1 */
        ensure_primes(end);
        seq_t sum = cum[end] - cum[n];   /* sum of primes n+1 .. n+k */
        if (sum % p == 0) return (seq_t)k;
        ++k;
    }
}
