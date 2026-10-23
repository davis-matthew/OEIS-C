/*
 * A124821  Number triangle T(n,k)=(-1)^(n-k)*(3k+2)*C(3n+1, n-k)/(2n+k+2).
 * https://oeis.org/A124821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   T(n,k)=(-1)^(n-k)*(3k+2)*C(3n+1, n-k)/(2n+k+2) a(n) is the
 *            flattened row‑major list of the triangle
 * Range:     i8 n <= 9, i16 n <= 35, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* ---------------------------------------------------------------- */
/* Prime table up to 30000 (max N = 3*10000+1)                       */
/* ---------------------------------------------------------------- */
#define MAX_PRIME    30000
#define MAX_PRIMES   3245   /* number of primes <= 30000 */

static int  primes[MAX_PRIMES];      /* prime list */
static int  prime_index[MAX_PRIME+1];/* index of a prime in primes[], -1 if not prime */
static int  prime_cnt = 0;           /* number of primes stored */
static bool primes_ready = false;

/* initialise prime tables once */
static void init_primes(void)
{
    if (primes_ready) return;

    /* sieve on stack */
    bool is_prime[MAX_PRIME+1];
    for (int i = 0; i <= MAX_PRIME; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= MAX_PRIME; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i) is_prime[j] = false;
        }
    }

    for (int i = 0; i <= MAX_PRIME; ++i) prime_index[i] = -1;
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (is_prime[i]) {
            primes[prime_cnt] = i;
            prime_index[i] = prime_cnt;
            ++prime_cnt;
        }
    }
    primes_ready = true;
}

/* factorise a small (<=MAX_PRIME) integer and add its prime exponents
   to the array `exps` with the given sign (+1 or -1). */
static void factor_uint32(uint32_t term, int sign, int *exps)
{
    if (term <= 1) return;
    for (int i = 0; i < prime_cnt && (uint32_t)primes[i] * (uint32_t)primes[i] <= term; ++i) {
        int p = primes[i];
        while (term % p == 0) {
            exps[i] += sign;
            term /= p;
        }
    }
    if (term > 1) {          /* remaining prime > sqrt(original term) */
        int idx = prime_index[term];
        exps[idx] += sign;
    }
}

/* ---------------------------------------------------------------- */
/* Sequence function a(idx_t n)                                      */
/* ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_primes();

    /* find row r and column k from the flattened index n            */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (cumulative + (r + 1) <= n) {
        cumulative += r + 1;
        ++r;
    }
    idx_t k = n - cumulative;                 /* 0 <= k <= r */
    idx_t K = r - k;                           /* size of binomial */

    /* exponent table for primes */
    int exps[MAX_PRIMES];
    for (int i = 0; i < prime_cnt; ++i) exps[i] = 0;

    /* binomial coefficient C(3r+1, K) via prime factorisation       */
    uint32_t N = 3 * (uint32_t)r + 1;
    for (idx_t i = 1; i <= K; ++i) {
        uint32_t num = N - K + (uint32_t)i;
        uint32_t den = (uint32_t)i;
        factor_uint32(num, +1, exps);
        factor_uint32(den, -1, exps);
    }

    /* extra factor (3k+2) in numerator */
    factor_uint32((uint32_t)(3 * k + 2), +1, exps);

    /* denominator (2r + k + 2) */
    factor_uint32((uint32_t)(2 * r + k + 2), -1, exps);

    /* compute the integer result from prime exponents                */
    seq_t result = 1;
    for (int i = 0; i < prime_cnt; ++i) {
        int e = exps[i];
        if (e <= 0) continue;
        seq_t p = (seq_t)primes[i];
        for (int h = 0; h < e; ++h) result = result * p;
    }

    /* apply sign (-1)^(r-k) */
    if ((r - k) & 1U) result = -result;

    return result;
}
