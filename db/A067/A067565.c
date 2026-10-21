/*
 * A067565  Inverse of R. L. Graham's sequence (A006255), or zero if and only
 *          if n is a prime.
 * https://oeis.org/A067565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest m ≤ n such that there exists a * increasing
 *            chain m = a1 < a2 < … < at = n with product a1*a2*…*at * being a
 *            perfect square. a(n)=0 iff n is prime. For perfect squares *
 *            a(n)=n. The function below computes this for 1 ≤ n ≤ 10000 * by
 *            linear‑algebra over GF(2) on the exponents mod...
 * Range:     u8 n <= 255, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* limits */
#define MAX_N 10000                     /* maximum n handled */
#define MAX_PR 1229                     /* number of primes ≤ 10000 */
#define WORDS ((MAX_PR + 63) / 64)      /* words needed per bit vector */

/* global caches, initialised on first call */
static bool initialized = false;
static uint64_t vector_bits[MAX_N + 1][WORDS];
static bool is_square[MAX_N + 1];
static int primes[MAX_PR];
static int prime_cnt;
static int prime_index[MAX_N + 1];      /* maps prime value → its index, -1 if not prime */

/* integer square root, floor of sqrt(n) */
static uint64_t int_sqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFUL;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* initialise primes and bit vectors */
static void init(void)
{
    /* sieve primes up to MAX_N */
    bool is_prime[MAX_N + 1];
    memset(is_prime, true, sizeof(is_prime));
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= MAX_N; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= MAX_N; j += i)
                is_prime[j] = false;
        }
    }
    prime_cnt = 0;
    for (int i = 2; i <= MAX_N; ++i) {
        if (is_prime[i]) {
            primes[prime_cnt++] = i;
            prime_index[i] = prime_cnt - 1;
        } else {
            prime_index[i] = -1;
        }
    }

    /* precompute squarefree parity vectors and square flag */
    for (int n = 1; n <= MAX_N; ++n) {
        uint64_t v[WORDS] = {0};
        int temp = n;
        for (int pi = 0; pi < prime_cnt && primes[pi] * primes[pi] <= temp; ++pi) {
            int p = primes[pi];
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (exp & 1) {
                int word = pi >> 6;
                int bit = pi & 63;
                v[word] ^= (1ULL << bit);
            }
        }
        if (temp > 1) {                    /* remaining large prime factor */
            int pi = prime_index[temp];
            if (pi >= 0) {
                int word = pi >> 6;
                int bit = pi & 63;
                v[word] ^= (1ULL << bit);
            }
        }
        memcpy(vector_bits[n], v, WORDS * sizeof(uint64_t));

        /* square flag */
        uint64_t r = int_sqrt_uint64(n);
        is_square[n] = (r * r == (uint64_t)n);
    }

    initialized = true;
}

/* helper: find highest set bit in v and return its global index;
 * returns -1 if v is zero. */
static inline int leading_bit(const uint64_t *v)
{
    for (int w = WORDS - 1; w >= 0; --w) {
        if (v[w]) {
            int bit = 63 - __builtin_clzll(v[w]); /* position 0..63 */
            return w * 64 + bit;
        }
    }
    return -1;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0 || n > MAX_N)          /* out of supported range */
        return (seq_t)0;

    if (is_square[n])
        return (seq_t)n;               /* perfect square case */

    /* linear basis over GF(2) */
    static uint64_t basis[WORDS * 64][WORDS];
    static int idx_to_basis[WORDS * 64];
    static int basis_count;

    /* initialise basis */
    basis_count = 0;
    for (int i = 0; i < WORDS * 64; ++i)
        idx_to_basis[i] = -1;

    const uint64_t *vec_n = vector_bits[n];

    for (idx_t m = n - 1; m >= 1; --m) {
        /* target = vec_n XOR vec_m */
        uint64_t target[WORDS];
        const uint64_t *vec_m = vector_bits[(int)m];
        for (int w = 0; w < WORDS; ++w)
            target[w] = vec_n[w] ^ vec_m[w];

        /* check if target is in span of current basis */
        uint64_t tmp[WORDS];
        memcpy(tmp, target, WORDS * sizeof(uint64_t));
        bool ok = true;
        while (true) {
            int lb = leading_bit(tmp);
            if (lb < 0) {            /* zero vector */
                ok = true;
                break;
            }
            int bi = idx_to_basis[lb];
            if (bi < 0) {
                ok = false;
                break;
            }
            for (int w = 0; w < WORDS; ++w)
                tmp[w] ^= basis[bi][w];
        }
        if (ok)
            return (seq_t)m;       /* found largest m */

        /* insert vec_m into basis */
        uint64_t v[WORDS];
        memcpy(v, vec_m, WORDS * sizeof(uint64_t));
        /* reduce v using existing basis */
        while (true) {
            int lb = leading_bit(v);
            if (lb < 0)
                break;          /* zero vector, dependent -> skip */
            int bi = idx_to_basis[lb];
            if (bi < 0)
                break;          /* new leading bit */
            for (int w = 0; w < WORDS; ++w)
                v[w] ^= basis[bi][w];
        }
        int lead = leading_bit(v);
        if (lead < 0)
            continue;           /* dependent, no new basis vector */
        /* add new basis vector */
        int idx = basis_count++;
        for (int w = 0; w < WORDS; ++w)
            basis[idx][w] = v[w];
        idx_to_basis[lead] = idx;
    }

    return (seq_t)0; /* prime case */
}
