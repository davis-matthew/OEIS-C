/*
 * A071328  Smallest prime q such that q - prime(n) >= n.
 * https://oeis.org/A071328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime q such that q - p_n >= n, * where p_n
 *            is the n-th prime (n ≥ 1, offset 1)
 * Range:     u8 n <= 46, u16 n <= 6016, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static list of found primes and expands it only
 * as needed.  The next prime ≥ X is found by simple trial division by the
 * already known primes; this is fast enough for the problem limits. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- static prime cache ---------------------------------------------- */
static size_t prime_cnt  = 0;      /* number of primes currently stored     */
static size_t prime_cap  = 0;      /* allocated capacity                    */
static uint64_t *primes  = NULL;  /* array of primes (uint64_t for safety) */
static uint64_t last_checked = 1;  /* last integer tested for primality    */

/* enlarge the stored array of primes (doubling capacity) */
static void grow_primes(void)
{
    if (prime_cap == 0) {
        prime_cap = 16;
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else {
        prime_cap <<= 1;
        primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
    }
    if (!primes) abort();  /* failure is fatal */
}

/* append a prime to the cache */
static void push_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) grow_primes();
    primes[prime_cnt++] = p;
}

/* test primality of x using only the primes already known (trial division) */
static bool is_prime_local(uint64_t x)
{
    if (x < 2) return false;
    if (prime_cnt == 0) return (x == 2);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* generate the next prime after the current cache */
static void generate_next_prime(void)
{
    uint64_t cand = last_checked + 1;
    /* make sure we test only odd candidates, except for 2 */
    if (cand <= 2) cand = 2;
    else if ((cand & 1ULL) == 0) ++cand;

    for (;; cand += 2) {
        if (is_prime_local(cand)) {
            push_prime(cand);
            last_checked = cand;
            return;
        }
    }
}

/* ensure that we know at least 'need' primes (1‑based index) */
static void ensure_n_primes(idx_t need)
{
    while (prime_cnt < (size_t)need) generate_next_prime();
}

/* return the smallest prime ≥ start  */
static seq_t next_prime_geq(seq_t start)
{
    if (start <= 2) return (seq_t)2;
    uint64_t cand = (uint64_t)start;
    if ((cand & 1ULL) == 0) ++cand;          /* start with odd number */

    for (;;) {
        if (is_prime_local(cand)) return (seq_t)cand;
        cand += 2;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour for out‑of‑range indices is undefined */

    /* obtain p_n (nth prime) */
    ensure_n_primes(n);
    uint64_t prime_n = primes[n - 1];  /* primes array is 0‑based */

    /* smallest prime ≥ prime_n + n */
    seq_t target = (seq_t)prime_n + (seq_t)n;
    return next_prime_geq(target);
}
