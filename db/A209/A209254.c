/*
 * A209254  Number of ways to write 2n-1 = p+q with q practical, p and p^4+q^4
 *          both prime.
 * https://oeis.org/A209254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { p prime, q practical : 2n-1 = p+q and p^4+q^4 prime }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- 64‑bit deterministic Miller–Rabin ---------- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); i++) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    /* bases for deterministic test up to 2^64 */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); i++) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; r++) {
            x = mul_mod(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Simple sieve up to MAX_N to get primes ----------
   We need primes up to 20000 (max 2n-1 for n<=10000).          */
#define MAX_N 20000
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

static void init_primes(void) {
    if (primes) return;
    bool *sieve = (bool*)calloc(MAX_N + 1, sizeof(bool));
    primes_cnt = 0;
    for (size_t i = 2; i <= MAX_N; i++)
        if (!sieve[i]) {
            primes_cnt++;
            for (size_t j = i * 2; j <= MAX_N; j += i) sieve[j] = true;
        }
    primes = (uint64_t*)malloc(primes_cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= MAX_N; i++) if (!sieve[i]) primes[idx++] = i;
    free(sieve);
}

/* ---------- check if n is practical ----------
   (only needed for n <= MAX_N).  */
static bool is_practical(uint64_t n) {
    if (n <= 1) return true;           /* 1 is practical by definition */
    if (n == 2) return true;           /* 2 is practical */

    /* Factor n using trial division (all primes up to sqrt(n) are in primes[]) */
    uint64_t temp = n;
    uint64_t prev_prime = 0;
    uint64_t prod_sigma = 1;

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; i++) {
        uint64_t p = primes[i];
        if (temp % p) continue;
        if (prev_prime != 0 && p > prod_sigma + 1) return false;
        if (prev_prime == 0 && p != 2) return false;  /* first prime must be 2 */
        prev_prime = p;

        /* count exponent */
        uint64_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }
        /* sigma for p^exp: (p^(exp+1)-1)/(p-1) */
        uint64_t pi_pow = 1;
        for (uint64_t k = 0; k < exp + 1; k++) pi_pow *= p;
        uint64_t sigma_p = (pi_pow - 1) / (p - 1);
        prod_sigma *= sigma_p;
    }
    if (temp > 1) { /* remaining prime factor */
        if (prev_prime != 0 && temp > prod_sigma + 1) return false;
        if (prev_prime == 0 && temp != 2) return false;
        /* exponent is 1 */
        uint64_t p = temp;
        uint64_t exp = 1;
        uint64_t pi_pow = p * p; /* p^(1+1) */
        uint64_t sigma_p = (pi_pow - 1) / (p - 1);
        prod_sigma *= sigma_p;
    }
    return true;
}

/* ---------- cache for practical numbers ----------
   We precompute an array of bool of size MAX_N+1. */
static bool *practical_cache = NULL;

static void init_practical_cache(void) {
    if (practical_cache) return;
    practical_cache = (bool*)malloc((MAX_N + 1) * sizeof(bool));
    for (uint64_t i = 0; i <= MAX_N; i++) practical_cache[i] = is_practical(i);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    /* Ensure prime list and practical array are initialized once. */
    init_primes();
    init_practical_cache();

    uint64_t target = 2 * n - 1;     /* 2n-1 is odd <= 19999 */
    seq_t ways = 0;

    /* iterate over primes p <= target */
    for (size_t i = 0; i < primes_cnt; i++) {
        uint64_t p = primes[i];
        if (p > target) break;
        uint64_t q = target - p;
        if (q < 1) continue;
        if (q > MAX_N) continue; /* safety, though not needed */
        if (!practical_cache[q]) continue;

        /* Compute p^4 + q^4 */
        uint64_t p2 = p * p;
        uint64_t p4 = p2 * p2;
        uint64_t q2 = q * q;
        uint64_t q4 = q2 * q2;
        uint64_t sum = p4 + q4;
        if (is_prime_uint64(sum)) ways++;
    }

    return ways;
}
