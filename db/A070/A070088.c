/*
 * A070088  Number of integer-sided triangles with perimeter n and prime
 *          sides.
 * https://oeis.org/A070088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of integer‑sided triangles with perimeter n and
 *            all three side lengths prime, counted up to ordering (i.e. with
 *            sides a ≤ b ≤ c)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build a simple sieve and store the list of primes.  The routine ensures that the sieve
   includes all primes up to the given limit.  It is re‑used by every call to the sequence. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static size_t sieve_limit = 0;
static uint8_t *isPrime = NULL;          /* 1 if prime, 0 otherwise, indexed by number */
static uint32_t *prime_list = NULL;      /* array of primes in ascending order */
static size_t  prime_cnt     = 0;
static size_t  prime_cap     = 0;

static void extend_sieve(idx_t n)
{
    if ((size_t)n <= sieve_limit) return;            /* already sufficient */

    size_t new_limit = (size_t)n;
    /* allocate or grow the boolean array */
    uint8_t *new_isPrime = (uint8_t*)realloc(isPrime, new_limit + 1);
    if (!new_isPrime) abort();                       /* crash on OOM, acceptable for contest */
    isPrime = new_isPrime;
    memset(isPrime + sieve_limit + 1, 1, new_limit - sieve_limit);

    /* Classic Sieve of Eratosthenes */
    if (new_limit >= 0) isPrime[0] = 0;
    if (new_limit >= 1) isPrime[1] = 0;
    for (size_t i = 2; i * i <= new_limit; ++i)
        if (isPrime[i])
            for (size_t j = i * i; j <= new_limit; j += i)
                isPrime[j] = 0;

    /* rebuild prime list */
    prime_cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i) {
        if (isPrime[i]) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 64;
                uint32_t *tmp = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = (uint32_t)i;
        }
    }
    sieve_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function  */
static inline seq_t count_prime_triangles(idx_t n)
{
    extend_sieve(n);                      /* guarantees primes and sieve up to n    */
    seq_t count = 0;

    /* iterate over primes p1 <= p2 <= p3 with sum n */
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= n / 3; ++i) {
        uint32_t p1 = prime_list[i];
        for (size_t j = i; j < prime_cnt; ++j) {
            uint32_t p2 = prime_list[j];
            uint64_t sum12 = (uint64_t)p1 + (uint64_t)p2;
            if (sum12 > n) break;                 /* p1+p2 too large, no further p2 work   */
            uint64_t p3 = n - sum12;              /* remaining side length                 */
            if (p3 < p2) break;                   /* would violate ordering p2 <= p3        */
            if (p3 <= sieve_limit && isPrime[(size_t)p3]) {
                if (sum12 > p3)                   /* triangle inequality  (p1+p2>p3)      */
                    ++count;
            }
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    /* a(1) = 0 ... definition handled by count_prime_triangles */
    if (n < 3) return 0;                 /* triangle with perimeter < 6 impossible */
    return count_prime_triangles(n);
}
