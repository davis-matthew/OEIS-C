/*
 * A158641  Strong primes p: adding 2 to any one digit of p produces a prime
 *          number (no digits 8 & 9 in p).
 * https://oeis.org/A158641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A158641: Strong primes. 
 * For every decimal digit of p that is not 8 or 9,
 * adding 2 to that single digit yields a prime.
 * The sequence is finite; the largest term has 5 digits (32057).
 * This function precomputes all terms up to the maximum required value
 * (99999) and returns the n‑th term (1‑based).
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* maximum prime value needed for the search */
#define SIEVE_LIMIT 120000
/* maximum number of primes up to the sieve limit */
#define MAX_PRIMES 12000

/* allocated array of all primes up to SIEVE_LIMIT */
static uint64_t *global_primes = NULL;
static size_t   global_prime_cnt = 0;

/* array of strong primes (in seq_t) */
static seq_t *strong_primes = NULL;
static size_t strong_cnt = 0;
static bool initialized = false;

/* simple sieve of Eratosthenes using a static bit array */
static void build_sieve(void)
{
    static unsigned char comp[SIEVE_LIMIT + 1] = {0};
    size_t i, j, limit = SIEVE_LIMIT;
    for (i = 2; i * i <= limit; ++i)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i)
                comp[j] = 1;

    /* allocate array for primes */
    global_primes = (uint64_t *)malloc(MAX_PRIMES * sizeof(uint64_t));
    global_prime_cnt = 0;
    for (i = 2; i <= limit && global_prime_cnt < MAX_PRIMES; ++i)
        if (!comp[i])
            global_primes[global_prime_cnt++] = i;
}

/* primality test using the sieve for numbers <= SIEVE_LIMIT,
 * otherwise simple trial division */
static bool is_prime_uint64(uint64_t n)
{
    if (n <= 1) return false;
    if (n <= SIEVE_LIMIT) {
        if (n <= 1) return false;
        if (n == 2) return true;
        /* check in sieve array */
        unsigned char *comp = (unsigned char *)global_primes; /* not used */
        /* We already have a list of primes; just binary search */
        size_t lo = 0, hi = global_prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (global_primes[mid] == n) return true;
            if (global_primes[mid] < n) lo = mid + 1;
            else hi = mid;
        }
        return false;
    }
    /* trial division up to sqrt(n) using primes list */
    for (size_t k = 0; k < global_prime_cnt && global_primes[k] * global_primes[k] <= n; ++k)
        if (n % global_primes[k] == 0)
            return false;
    return true;
}

/* number of decimal digits of a uint64_t value */
static int digits_of_uint64(uint64_t x)
{
    int d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

/* check if a seq_t value has any digit 8 or 9 */
static bool bad_digit(seq_t v)
{
    uint64_t x = (uint64_t)v;
    while (x) {
        uint8_t d = x % 10;
        if (d == 8 || d == 9) return true;
        x /= 10;
    }
    return false;
}

/* initialize the global lists of primes and strong primes */
static void init_sequence(void)
{
    if (initialized) return;
    build_sieve();

    /* precompute powers of 10 */
    uint64_t pow10[6] = {1,10,100,1000,10000,100000};

    /* allocate space for strong primes (max 8325 for 120k limit) */
    strong_primes = (seq_t *)malloc(MAX_PRIMES * sizeof(seq_t));
    strong_cnt = 0;

    for (size_t i = 0; i < global_prime_cnt; ++i) {
        uint64_t p = global_primes[i];
        if (p > 99999) break;          /* we only care primes up to 99999 */
        if (bad_digit(p)) continue;

        int len = digits_of_uint64(p);
        bool ok = true;
        for (int pos = 0; pos < len; ++pos) {
            uint64_t np = p + 2ULL * pow10[pos];
            if (!is_prime_uint64(np)) { ok = false; break; }
        }
        if (ok) {
            if (strong_cnt == MAX_PRIMES) {
                /* should never happen for this sequence */
                strong_primes = (seq_t *)realloc(strong_primes,
                                                 strong_cnt * 2 * sizeof(seq_t));
            }
            strong_primes[strong_cnt++] = (seq_t)p;
        }
    }
    initialized = true;
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, n<1 undefined */
    init_sequence();
    if ((idx_t)n > strong_cnt) return 0;
    return strong_primes[n - 1];
}
