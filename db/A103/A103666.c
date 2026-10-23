/*
 * A103666  Primes p such that the largest prime divisor of p-1 is less than
 *          the largest prime divisor of p+1.
 * https://oeis.org/A103666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime p such that largest prime factor(p-1) < largest
 *            prime factor(p+1)
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime list handling ---------- */
static uint64_t *prime_list = NULL;    /* primes, 2,3,5,...   */
static size_t prime_count   = 0;       /* number of primes    */
static size_t prime_cap     = 0;       /* capacity of array   */
static uint64_t prime_candidate = 3;   /* next odd to test    */

/* initialize prime list with the first prime 2 */
static void init_primes(void)
{
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }
    prime_list[0] = 2;
    prime_count = 1;
    prime_candidate = 3;
}

/* enlarge prime array if necessary */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need) return;
    size_t newcap = prime_cap ? prime_cap * 2 : 16;
    while (newcap < need) newcap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, newcap * sizeof(uint64_t));
    if (tmp) {
        prime_list = tmp;
        prime_cap = newcap;
    }
}

/* get the next prime; updates prime_list */
static uint64_t get_next_prime(void)
{
    if (prime_count == 0) init_primes();

    while (1) {
        uint64_t cand = prime_candidate;
        bool isprime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if ((__uint128_t)p * p > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }
        prime_candidate += 2;   /* next odd to test */

        if (isprime) {
            ensure_prime_cap(prime_count + 1);
            prime_list[prime_count++] = cand;
            return cand;
        }
    }
}

/* ensure primes are known up to sqrt(n) */
static void extend_primes_sufficient(uint64_t n)
{
    if (prime_count == 0) init_primes();
    uint64_t last = prime_list[prime_count - 1];
    while ((__uint128_t)last * last <= n) {
        last = get_next_prime();
    }
}

/* ---------- largest prime factor of n (n>=2) ---------- */
static uint64_t largest_prime_factor(uint64_t n)
{
    uint64_t rem = n;
    uint64_t largest = 0;
    extend_primes_sufficient(n);
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if ((__uint128_t)p * p > rem) break;
        if (rem % p == 0) {
            largest = p;
            while (rem % p == 0) rem /= p;
        }
    }
    if (rem > 1) largest = rem;
    return largest;
}

/* ---------- Term list handling ---------- */
static seq_t *term_list = NULL;     /* stored primes satisfying condition */
static size_t term_count = 0;       /* number of terms stored */
static size_t term_cap   = 0;       /* capacity of array */
static uint64_t last_checked_prime = 2; /* last prime processed */

/* enlarge term array if needed */
static void ensure_term_cap(size_t need)
{
    if (term_cap >= need) return;
    size_t newcap = term_cap ? term_cap * 2 : 16;
    while (newcap < need) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(term_list, newcap * sizeof(seq_t));
    if (tmp) {
        term_list = tmp;
        term_cap = newcap;
    }
}

/* populate term list up to at least index 'need' (1‑based) */
static void ensure_terms(size_t need)
{
    if (term_count >= need) return;
    while (term_count < need) {
        uint64_t p = get_next_prime();     /* get next prime > last_checked_prime */
        last_checked_prime = p;
        if (p <= 2) continue;              /* ignore 2 */

        uint64_t lpd_minus = largest_prime_factor(p - 1);
        uint64_t lpd_plus  = largest_prime_factor(p + 1);

        if (lpd_minus < lpd_plus) {
            ensure_term_cap(term_count + 1);
            term_list[term_count++] = (seq_t)p;
        }
    }
}

/* ---------- exported function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safe default for out‑of‑range */
    ensure_terms((size_t)n);
    return term_list[n - 1];
}
