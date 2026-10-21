/*
 * A075285  Interprimes which are of the form s*prime, s=10.
 * https://oeis.org/A075285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075285 – Interprimes of the form s*prime, s = 10.
   An interprime is the arithmetic mean of two consecutive prime numbers.
   For every n ≥ 1 the function returns the n‑th such number. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/*------------------------------------------------------------*/
/* Dynamic arrays of uint64_t for primes and interprimes.    */
/*------------------------------------------------------------*/
static uint64_t *primes = NULL;
static size_t   primes_sz = 0, primes_cap = 0;

static uint64_t *inters = NULL;
static size_t   inters_sz = 0, inters_cap = 0;

/* Allocate or grow an array to hold at least 'need' elements. */
static void grow_array(uint64_t **arr, size_t *sz, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        while (newcap < need) newcap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        *arr = tmp;
        *cap = newcap;
    }
}

/* Append a value to a dynamic array. */
static void push_back(uint64_t **arr, size_t *sz, size_t *cap, uint64_t val)
{
    grow_array(arr, sz, cap, *sz + 1);
    (*arr)[(*sz)++] = val;
}

/*------------------------------------------------------------*/
/* Prime helpers.                                            */
/*------------------------------------------------------------*/

/* Ensure that all primes up to at least 'limit' are generated. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (primes_sz == 0) {
        push_back(&primes, &primes_sz, &primes_cap, 2ull);
    }
    if (primes[primes_sz - 1] >= limit) return;

    for (uint64_t cand = primes[primes_sz - 1] + 1; cand <= limit; ++cand) {
        uint64_t r = (uint64_t)sqrt((long double)cand);
        int isprime = 1;
        for (size_t i = 0; i < primes_sz && primes[i] <= r; ++i) {
            if (cand % primes[i] == 0) { isprime = 0; break; }
        }
        if (isprime) push_back(&primes, &primes_sz, &primes_cap, cand);
    }
}

/* Test whether 'n' is prime, expanding the prime table as needed. */
static int isPrime64(uint64_t n)
{
    if (n < 2) return 0;
    uint64_t r = (uint64_t)sqrt((long double)n);
    ensure_primes_up_to(r);
    for (size_t i = 0; i < primes_sz && primes[i] <= r; ++i) {
        if (n % primes[i] == 0) return 0;
    }
    return 1;
}

/* Return the smallest prime strictly greater than 'prev'. */
static uint64_t nextPrime(uint64_t prev)
{
    uint64_t cand = prev + 1;
    for (;;) {
        if (isPrime64(cand)) return cand;
        ++cand;
    }
}

/*------------------------------------------------------------*/
/* The required function.                                    */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Never initialise again; static variables retain state. */
    /* Build primes and interprimes until we have at least n entries. */
    while (inters_sz < n) {
        /* Ensure we have at least two primes to form a pair. */
        if (primes_sz < 2) {
            uint64_t nextp = (primes_sz == 0) ? 2ull : nextPrime(primes[primes_sz - 1]);
            push_back(&primes, &primes_sz, &primes_cap, nextp);
            continue;
        }

        uint64_t p_prev = primes[primes_sz - 2];
        uint64_t p_curr = primes[primes_sz - 1];

        /* Sum of the two primes. If sum is even, the average is integer. */
        uint64_t sum = p_prev + p_curr;
        if ((sum & 1ull) == 0) {
            uint64_t avg = sum >> 1;          /* (p_prev + p_curr) / 2 */
            if ((avg % 10ull) == 0) {         /* must be divisible by 10            */
                uint64_t q = avg / 10ull;    /* candidate prime factor              */
                if (isPrime64(q)) {
                    push_back(&inters, &inters_sz, &inters_cap, avg);
                }
            }
        }

        /* Generate the next prime to advance the sliding window. */
        uint64_t nextp = nextPrime(p_curr);
        push_back(&primes, &primes_sz, &primes_cap, nextp);
    }

    return (seq_t)inters[n - 1];
}
