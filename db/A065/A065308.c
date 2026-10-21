/*
 * A065308  a(n) = prime(n - pi(n)).
 * https://oeis.org/A065308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n - pi(n)) pi(n) = number of primes ≤ n. The code
 *            builds primes on demand using a simple trial‑division sieve and
 *            caches them in a growing array. It works for any n ≥ 1 and
 *            returns the value cast to seq_t
 * Range:     u8 n <= 75, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static uint64_t *prime_arr = NULL;   /* array of discovered primes */
static size_t prime_cnt  = 0;        /* number of primes stored   */
static size_t prime_cap  = 0;        /* current array capacity    */

/* Ensure that prime_arr contains at least all primes ≤ limit.   */
static void generate_primes_up_to (uint64_t limit)
{
    if (prime_arr == NULL) {
        prime_cap = 1024;
        prime_arr = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_cnt = 0;
    }

    if (prime_cnt == 0) {
        prime_arr[prime_cnt++] = 2;   /* first prime */
    }

    uint64_t last = prime_arr[prime_cnt - 1];
    uint64_t cand = (last == 2 ? 3 : last + 2);

    while (cand <= limit) {
        int is_prime = 1;
        uint64_t r = (uint64_t)floor(sqrt((long double)cand));
        for (size_t i = 0; i < prime_cnt && prime_arr[i] <= r; ++i) {
            if (cand % prime_arr[i] == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_arr = (uint64_t *)realloc(prime_arr,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_arr[prime_cnt++] = cand;
        }
        cand += 2;
    }
}

/* Return the number of primes ≤ n (i.e. π(n)).  */
static idx_t prime_count_leq (idx_t n)
{
    generate_primes_up_to (n);
    /* count primes ≤ n */
    size_t c = 0;
    while (c < prime_cnt && prime_arr[c] <= n) ++c;
    return (idx_t)c;
}

/* Ensure that prime_arr holds at least ‘need’ primes (index ≥ need). */
static void ensure_prime_cnt (idx_t need)
{
    if (prime_cnt >= (size_t)need) return;

    uint64_t cand = prime_arr[prime_cnt - 1] + 2;
    while (prime_cnt < (size_t)need) {
        int is_prime = 1;
        uint64_t r = (uint64_t)floor(sqrt((long double)cand));
        for (size_t i = 0; i < prime_cnt && prime_arr[i] <= r; ++i) {
            if (cand % prime_arr[i] == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_arr = (uint64_t *)realloc(prime_arr,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_arr[prime_cnt++] = cand;
        }
        cand += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range, harmless */
    idx_t pi_n = prime_count_leq (n);
    idx_t m    = n - pi_n;                /* index of the required prime */
    ensure_prime_cnt (m);
    return (seq_t)prime_arr[m - 1];
}
