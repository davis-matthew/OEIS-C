/*
 * A235924  a(n) = |{0 < k < n: p = phi(k) + phi(n-k)/3 + 1, q = prime(p) - p
 *          + 1 and r = prime(q) - q + 1 are all prime}|, where phi(.) is
 *          Euler's totient function.
 * https://oeis.org/A235924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n: p = φ(k) + φ(n-k)/3 + 1, q = prime(p) – p +
 *            1, r = prime(q) – q + 1 are all prime}| where φ is Euler's
 *            totient and prime(m) is the m‑th prime
 * Range:     u8 n <= 35, u16 n <= 1741, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes φ(k) for k ≤ 20000 and a prime sieve
   up to 3 000 000.  For each n it scans all k < n, checks the conditions
   and counts such k.  It works for all n ≥ 1, independent of call order. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_N          20000          /* pre‑compute φ up to this */
#define PRIME_LIMIT    3000000        /* sieve up to this to get all needed primes */

static seq_t *phi_arr = NULL;       /* φ values (index 0 unused) */
static bool   *isPrime  = NULL;     /* primality table */
static uint32_t *prime_list = NULL; /* list of primes */
static size_t  primesCount = 0;     /* number of primes in prime_list */

/* One‑time initialisation of all tables */
static bool init_done = false;

static void init(void)
{
    if (init_done) return;
    init_done = true;

    /* 1. Compute totients φ(1..MAX_N) using the standard sieve */
    phi_arr = (seq_t*)calloc(MAX_N + 1, sizeof(seq_t));
    if (!phi_arr) return; /* allocation failure, nothing we can do */
    phi_arr[1] = 1;
    for (idx_t i = 2; i <= MAX_N; ++i) phi_arr[i] = i;
    for (idx_t i = 2; i <= MAX_N; ++i) {
        if (phi_arr[i] == i) {               /* i is prime */
            for (idx_t j = i; j <= MAX_N; j += i) {
                phi_arr[j] -= phi_arr[j] / i;
            }
        }
    }

    /* 2. Prime sieve up to PRIME_LIMIT */
    isPrime = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!isPrime) return;
    for (idx_t i = 0; i <= PRIME_LIMIT; ++i) isPrime[i] = true;
    if (PRIME_LIMIT >= 0) isPrime[0] = false;
    if (PRIME_LIMIT >= 1) isPrime[1] = false;
    for (idx_t i = 2; (idx_t)i * i <= PRIME_LIMIT; ++i) {
        if (isPrime[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i) {
                isPrime[j] = false;
            }
        }
    }

    /* 3. Build prime list (dynamic array) */
    prime_list = (uint32_t*)malloc(250000 * sizeof(uint32_t)); /* initial capacity */
    if (!prime_list) return;
    size_t cap = 250000;
    primesCount = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (isPrime[i]) {
            if (primesCount == cap) {
                cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(prime_list, cap * sizeof(uint32_t));
                if (!tmp) return; /* allocation failure */
                prime_list = tmp;
            }
            prime_list[primesCount++] = (uint32_t)i;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init();            /* guarantees tables ready */

    seq_t count = 0;
    if (n <= 1) return 0;   /* offset 1: no k in (0,1) */

    for (idx_t k = 1; k < n; ++k) {
        seq_t phi_k   = phi_arr[k];
        seq_t phi_nk  = phi_arr[n - k];
        if (phi_nk % 3 != 0) continue;

        seq_t p = phi_k + phi_nk / 3 + 1;
        if (p < 2) continue;
        if ((size_t)p > PRIME_LIMIT) continue;
        if (!isPrime[(size_t)p]) continue;
        if ((size_t)p > primesCount) continue;          /* safety for p-th prime */

        seq_t prime_of_p = (seq_t)prime_list[(size_t)p - 1];
        seq_t q = prime_of_p - p + 1;
        if (q < 2 || (size_t)q > PRIME_LIMIT) continue;
        if (!isPrime[(size_t)q]) continue;
        if ((size_t)q > primesCount) continue;          /* safety for q-th prime */

        seq_t prime_of_q = (seq_t)prime_list[(size_t)q - 1];
        seq_t r = prime_of_q - q + 1;
        if (r < 2 || (size_t)r > PRIME_LIMIT) continue;
        if (!isPrime[(size_t)r]) continue;

        ++count;
    }
    return count;
}
