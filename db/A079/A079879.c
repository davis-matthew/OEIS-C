/*
 * A079879  Median prime factor: a(1)=1 and for n>1: least prime p such that
 *          not more than floor(Omega(n)/2) prime factors are greater than p;
 *          Omega(n) is the total number of prime factors of n (A001222).
 * https://oeis.org/A079879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = median prime factor of n (with multiplicity), defined as
 *            a(1)=1; for n>1, let Ω(n) be the total number of prime factors
 *            (with multiplicity). Return the least prime p such that at most
 *            ⌊Ω(n)/2⌋ prime factors are greater than p. This is the lower
 *            median of the sorted list of prime factors of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Prime sieve up to 1 000 000 (enough for n ≤ 10¹²) ---------- */
#define PRIME_LIMIT 1000000U
#define MAX_PRIMES  78498U          /* number of primes ≤ 1 000 000 */

static uint32_t primes[MAX_PRIMES];
static uint32_t primes_cnt = 0U;
static bool sieve_built = false;
static bool sieve_is_prime[PRIME_LIMIT + 1];

/* Build the prime list once.  This routine is re‑entrant but
   performs the sieve only on the first call. */
static void init_primes(void)
{
    if (sieve_built) return;
    for (uint32_t i = 0U; i <= PRIME_LIMIT; ++i) sieve_is_prime[i] = true;
    sieve_is_prime[0] = sieve_is_prime[1] = false;
    for (uint32_t p = 2U; p * p <= PRIME_LIMIT; ++p) {
        if (sieve_is_prime[p]) {
            for (uint32_t q = p * p; q <= PRIME_LIMIT; q += p)
                sieve_is_prime[q] = false;
        }
    }
    for (uint32_t i = 2U; i <= PRIME_LIMIT; ++i)
        if (sieve_is_prime[i]) primes[primes_cnt++] = i;
    sieve_built = true;
}

/* ---------- Median prime factor of n ---------- */
#define MAX_FACTORS 64U          /* maximum number of prime factors for n ≤ 10¹² */

static uint64_t median_prime_factor(uint64_t n)
{
    if (n == 1ULL) return 1ULL;        /* a(1)=1 */

    init_primes();

    uint64_t factors[MAX_FACTORS];
    uint32_t cnt = 0U;
    uint64_t m = n;

    for (uint32_t i = 0U; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        while (m % p == 0ULL) {
            factors[cnt++] = p;
            m /= p;
            if (cnt == MAX_FACTORS) break;   /* safety */
        }
    }
    if (cnt == MAX_FACTORS) return factors[(cnt - 1) / 2U]; /* should not happen */

    if (m > 1ULL) {
        /* m is a prime larger than PRIME_LIMIT */
        factors[cnt++] = m;
    }

    /* lower median: element at index floor((cnt-1)/2) in the sorted list */
    uint32_t median_index = (cnt - 1U) / 2U;
    return factors[median_index];
}

/* ---------- Public API required by the build ---------- */
seq_t a(idx_t n)
{
    return (seq_t)median_prime_factor((uint64_t)n);
}
