/*
 * A086324  Number of n X n circulant singular matrices over GF(2).
 * https://oeis.org/A086324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - A003473(n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow_seq, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003473(n) = number of nonsingular n×n binary circulant matrices. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow_uint.h"
#include "../utils/pow_seq.h"

#define PRIME_LIMIT 10000

static int g_primes[1500];
static size_t g_prime_cnt = 0;
static bool g_primes_done = false;

static void init_primes(void)
{
    if (g_primes_done) return;
    bool marked[PRIME_LIMIT + 1] = {false};
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!marked[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                marked[j] = true;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!marked[i]) g_primes[g_prime_cnt++] = i;
    g_primes_done = true;
}

static idx_t pow2_mod(idx_t exp, idx_t mod)
{
    idx_t res = 1;
    idx_t base = 2 % mod;
    while (exp) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

static idx_t multiplicative_order_of_2(idx_t d)
{
    if (d == 1) return 1;
    init_primes();

    /* compute φ(d) */
    idx_t phi = d;
    idx_t t = d;
    for (size_t i = 0; i < g_prime_cnt && (idx_t)g_primes[i] * (idx_t)g_primes[i] <= t; ++i) {
        int p = g_primes[i];
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            phi = phi / p * (p - 1);
        }
    }
    if (t > 1) {
        int p = (int)t;
        phi = phi / p * (p - 1);
    }

    /* distinct prime factors of φ(d) */
    idx_t phi_temp = phi;
    idx_t primes_phi[64];
    size_t pc = 0;
    for (size_t i = 0; i < g_prime_cnt && (idx_t)g_primes[i] * (idx_t)g_primes[i] <= phi_temp; ++i) {
        int p = g_primes[i];
        if (phi_temp % p == 0) {
            primes_phi[pc++] = p;
            while (phi_temp % p == 0) phi_temp /= p;
        }
    }
    if (phi_temp > 1) primes_phi[pc++] = phi_temp;

    /* test each prime factor */
    idx_t ord = phi;
    for (size_t i = 0; i < pc; ++i) {
        idx_t p = primes_phi[i];
        while (ord % p == 0) {
            if (pow2_mod(ord / p, d) == 1)
                ord /= p;
            else
                break;
        }
    }
    return ord;
}

static idx_t totient_val(idx_t n)
{
    if (n == 0) return 0;
    init_primes();
    idx_t result = n;
    idx_t t = n;
    for (size_t i = 0; i < g_prime_cnt && (idx_t)g_primes[i] * (idx_t)g_primes[i] <= t; ++i) {
        int p = g_primes[i];
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            result = result / p * (p - 1);
        }
    }
    if (t > 1) {
        int p = (int)t;
        result = result / p * (p - 1);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* not used in the sequence offset */

    init_primes();

    /* odd part of n */
    idx_t odd = n;
    while ((odd & 1) == 0) odd >>= 1;

    /* collect odd divisors of odd */
    idx_t divisors[256];
    size_t dcnt = 0;
    for (idx_t d = 1; d * d <= odd; ++d) {
        if (odd % d == 0) {
            divisors[dcnt++] = d;
            if (d != odd / d)
                divisors[dcnt++] = odd / d;
        }
    }

    seq_t P = (seq_t)1;
    idx_t S = 0;

    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divisors[i];
        idx_t deg = multiplicative_order_of_2(d);      /* degree of the minimal polynomial */
        idx_t phi_d = totient_val(d);                 /* Euler's totient of d */
        idx_t cnt = phi_d / deg;                      /* how many distinct irreducibles of this degree */

        S += deg * cnt;

        seq_t term = pow_uint(2, deg) - (seq_t)1;
        seq_t fac = pow_seq(term, (int)cnt);
        P *= fac;
    }

    seq_t pow2_n = pow_uint(2, n);
    seq_t pow2_n_minus_S = pow_uint(2, n - S);
    seq_t nonsingular = pow2_n_minus_S * P;
    seq_t result = pow2_n - nonsingular;
    return result;
}
