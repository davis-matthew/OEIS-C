/*
 * A129567  a(n) = number of positive divisors of the denominator of the n-th
 *          harmonic number H(n) = Sum_{k=1..n} 1/k.
 * https://oeis.org/A129567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of the denominator of the n‑th
 *            harmonic number H(n) = Σ_{k=1..n} 1/k * a(n) =
 *            A000005(A002805(n)) according to OEIS * Implementation uses
 *            modular arithmetic with small moduli (p^e) to determine the
 *            p‑adic valuation * of the numerator of L·H(n), where L =
 *            lcm(1..n). The d...
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 108, u64 n <= 270,
 *            u128 n <= 665, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Auxiliary structure for primes */
static uint64_t *g_primes = NULL;
static size_t g_prime_cnt = 0;
static size_t g_prime_capacity = 0;
static uint64_t g_max_computed = 0;

/* Simple sieve to compute primes up to limit */
static void sieve_primes(uint64_t limit) {
    if (limit <= g_max_computed)
        return;
    uint64_t new_cap = limit + 1;
    uint8_t *is_comp = (uint8_t *)calloc(new_cap, sizeof(uint8_t));
    if (!is_comp) abort();
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }
    size_t new_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            ++new_cnt;
    if (g_prime_capacity < new_cnt) {
        uint64_t *tmp = (uint64_t *)realloc(g_primes, new_cnt * sizeof(uint64_t));
        if (!tmp) abort();
        g_primes = tmp;
        g_prime_capacity = new_cnt;
    }
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            g_primes[idx++] = i;
    g_prime_cnt = new_cnt;
    g_max_computed = limit;
    free(is_comp);
}

/* Extended Euclid to compute modular inverse (works for coprime a, mod) */
static uint64_t mod_inverse(uint64_t a, uint64_t mod) {
    int64_t t = 0, newt = 1;
    int64_t r = mod, newr = a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = newt;
        newt = t - q * newt;
        t = tmp;
        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }
    if (r > 1) return 0; /* not invertible, shouldn't happen for coprime */
    if (t < 0) t += mod;
    return (uint64_t)t;
}

/* Fast modular power (small mod) */
static uint64_t pow_mod_small(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n <= 1) {
        return 1;
    }

    /* Compute all primes up to n once */
    sieve_primes((uint64_t)n);

    seq_t ans = 1;

    /* For each prime p ≤ n */
    for (size_t pi = 0; pi < g_prime_cnt && g_primes[pi] <= n; ++pi) {
        uint64_t p = g_primes[pi];

        /* Compute e = floor(log_p(n)) using successive division */
        uint64_t t = n;
        uint64_t e = 0;
        while (t >= p) {
            ++e;
            t /= p;
        }
        /* modulus = p^e (fits in 64-bit because p^e ≤ n) */
        uint64_t mod = 1;
        for (uint64_t k = 0; k < e; ++k) mod *= p;

        /* Compute L_rest_mod = product_{q≠p} q^{floor_log_q(n)} mod mod */
        uint64_t L_rest_mod = 1 % mod;
        for (size_t q_index = 0; q_index < g_prime_cnt && g_primes[q_index] <= n; ++q_index) {
            uint64_t q = g_primes[q_index];
            if (q == p) continue;
            /* f_q = floor(log_q(n)) */
            uint64_t tt = n;
            uint64_t f_q = 0;
            while (tt >= q) { ++f_q; tt /= q; }
            uint64_t qp = pow_mod_small(q, f_q, mod);
            L_rest_mod = (L_rest_mod * qp) % mod;
        }

        /* Pre‑compute p^i mod mod for i = 0..e to avoid repeated pow in the loop */
        uint64_t *pow_p = (uint64_t *)malloc((e + 1) * sizeof(uint64_t));
        pow_p[0] = 1 % mod;
        for (uint64_t i = 1; i <= e; ++i) {
            pow_p[i] = (pow_p[i - 1] * p) % mod;
        }

        /* Precompute inverses of numbers coprime to p modulo mod */
        uint64_t *inv = (uint64_t *)calloc((size_t)n + 1, sizeof(uint64_t));
        for (uint64_t m = 1; m <= n; ++m) {
            if (m % p == 0) continue;
            inv[m] = mod_inverse(m, mod);
            /* inv[m] should always be non‑zero for coprime pairs */
        }

        /* Compute S modulo mod: S = Σ_{k=1..n} L/k */
        uint64_t S_mod = 0;
        for (uint64_t k = 1; k <= n; ++k) {
            uint64_t kk = k;
            uint64_t t_p = 0;
            while ((kk % p) == 0) {
                ++t_p;
                kk /= p;
            }
            uint64_t k_prime = kk;           /* k with all p‑factors removed */
            uint64_t inv_kprime = inv[k_prime]; /* inverse modulo mod */

            uint64_t factor_p = pow_p[e - t_p];
            uint64_t term = (factor_p * ((L_rest_mod * inv_kprime) % mod)) % mod;
            S_mod += term;
            if (S_mod >= mod) S_mod -= mod;
        }

        /* Determine v = v_p(S) from S_mod */
        uint64_t temp = S_mod % mod;
        uint64_t v = 0;
        if (temp == 0) {
            v = e; /* S is divisible by p^e */
        } else {
            while (temp % p == 0) {
                temp /= p;
                ++v;
            }
        }
        uint64_t den_exp = e - v;          /* exponent of p in the reduced denominator */
        ans *= (seq_t)(den_exp + 1);       /* update divisor count */

        free(pow_p);
        free(inv);
    }

    return ans;
}
