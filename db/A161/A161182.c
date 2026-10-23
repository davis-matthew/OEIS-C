/*
 * A161182  Successive differences between positions of squares in list of
 *          nonprimes.
 * https://oeis.org/A161182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1) - (pi(n^2) - pi((n-1)^2)) where pi(x) is the
 *            number of primes ≤ x. The sequence is computed by sieving primes
 *            up to the required square range (default 10^8 = 10000^2)
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ---------- global data ---------- */
static seq_t *prime_list = NULL;   /* list of primes */
static size_t   prime_count = 0;   /* number of primes stored */
static uint64_t sieve_limit  = 0;  /* current sieve upper bound */

/* ---------- bitset helpers ---------- */
static inline void bit_set(uint8_t *arr, size_t idx) {
    arr[idx >> 3] |= (uint8_t)(1u << (idx & 7));
}
static inline int bit_get(const uint8_t *arr, size_t idx) {
    return (arr[idx >> 3] >> (idx & 7)) & 1;
}

/* ---------- sieve routine (generates primes up to 'limit') ---------- */
static void init_sieve(uint64_t limit) {
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }

    /* sieve is stored only for odd numbers: bit i represents number 2*i+1 */
    uint64_t half = limit >> 1;                     /* number of odds <= limit */
    size_t  bytes = (half >> 3) + 1;
    uint8_t *sieve = (uint8_t*)calloc(bytes, 1);
    if (!sieve) abort();                            /* memory failure */

    uint64_t sqrtL = (uint64_t)sqrt((double)limit);
    for (uint64_t i = 3; i <= sqrtL; i += 2) {
        if (!bit_get(sieve, i >> 1)) {
            for (uint64_t j = i * i; j <= limit; j += i << 1)
                bit_set(sieve, j >> 1);
        }
    }

    /* estimate prime count (~limit / log(limit)) for initial buffer */
    size_t est = (size_t)((double)limit / log((double)limit) * 1.2);
    if (est < 100) est = 100;
    seq_t *plist = (seq_t*)malloc(est * sizeof(seq_t));
    if (!plist) abort();

    size_t cnt = 0;
    plist[cnt++] = 2;                               /* prime 2 */
    for (uint64_t i = 3; i <= limit; i += 2) {
        if (!bit_get(sieve, i >> 1)) {
            if (cnt == est) {
                est <<= 1;
                plist = (seq_t*)realloc(plist, est * sizeof(seq_t));
                if (!plist) abort();
            }
            plist[cnt++] = (seq_t)i;
        }
    }

    prime_list  = plist;
    prime_count = cnt;
    sieve_limit = limit;
    free(sieve);
}

/* ---------- default initialisation ---------- */
static void init_default_primes(void) {
    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
        init_sieve(100000000ULL); /* 10^8 = (10000)^2, enough for n ≤ 10000 */
    }
}

/* ---------- prime counting function π(x) ---------- */
static inline size_t pi_u64(uint64_t x) {
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((uint64_t)prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* number of primes ≤ x */
}

/* ---------- the required sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety, though n≥1 in the OEIS offset */

    init_default_primes();

    uint64_t n_sq = (uint64_t)n * (uint64_t)n;
    uint64_t n1  = n - 1;
    uint64_t n1_sq = n1 * n1;

    /* If we haven't sieved far enough, extend the sieve */
    if (n_sq > sieve_limit) {
        init_sieve(n_sq);
    }

    size_t pi_n_sq  = pi_u64(n_sq);
    size_t pi_n1_sq = pi_u64(n1_sq);

    seq_t diff = (seq_t)((2ULL * n) - 1ULL);
    diff -= (seq_t)(pi_n_sq - pi_n1_sq);
    return diff;
}
