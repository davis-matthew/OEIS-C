/*
 * A110085  Numbers k such that sigma(k) - phi(k) < tau(k)^omega(k), where
 *          sigma = A000203, phi = A000010, tau = A000005 and omega = A001221.
 * https://oeis.org/A110085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest integer k such that sigma(k) – phi(k) <
 *            tau(k) ^ ω(k), where sigma = sum of divisors, phi = Euler’s
 *            totient, tau = number of divisors, ω = number of distinct prime
 *            factors
 * Range:     u8 none, u16 n <= 368, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *sigma_arr   = NULL;
static seq_t *phi_arr     = NULL;
static seq_t *tau_arr     = NULL;
static seq_t *omega_arr   = NULL;
static uint32_t *min_p    = NULL;

static seq_t *terms = NULL;          /* 0‑based: terms[0] = a(1) */
static idx_t terms_count = 0;

static const idx_t MAX_LIMIT = 400000;   /* sufficient for n ≤ 10000 */

/* Linear sieve that fills min_p with the smallest prime factor of each number */
static void init_sieve(idx_t limit)
{
    min_p   = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    sigma_arr = (seq_t*)calloc(limit + 1, sizeof(seq_t));
    phi_arr   = (seq_t*)calloc(limit + 1, sizeof(seq_t));
    tau_arr   = (seq_t*)calloc(limit + 1, sizeof(seq_t));
    omega_arr = (seq_t*)calloc(limit + 1, sizeof(seq_t));

    /* generate primes */
    uint32_t *primes = (uint32_t*)malloc((limit / 10 + 1) * sizeof(uint32_t));
    size_t   primes_size = 0;

    /* initialize 1 */
    sigma_arr[1] = 1;
    phi_arr[1]   = 1;
    tau_arr[1]   = 1;
    omega_arr[1] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (min_p[i] == 0) {
            min_p[i] = (uint32_t)i;
            if (primes_size == limit / 10) { /* grow array if needed */
                primes = (uint32_t*)realloc(primes, (size_t)(primes_size * 3 / 2 + 1) * sizeof(uint32_t));
            }
            primes[primes_size++] = (uint32_t)i;
        }

        for (size_t j = 0; j < primes_size && primes[j] <= min_p[i]; ++j) {
            idx_t v = i * (idx_t)primes[j];
            if (v > limit) break;
            min_p[v] = primes[j];
        }
    }

    /* compute multiplicative functions */
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t temp = i;
        seq_t sigma_i = 1;
        seq_t tau_i   = 1;
        seq_t omega_i = 0;
        seq_t phi_i   = (seq_t)i;

        while (temp > 1) {
            uint32_t p = min_p[temp];
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            /* sigma factor (p^(exp+1)-1)/(p-1) computed directly */
            seq_t power = 1;
            seq_t term  = 1;
            for (uint32_t k = 0; k < exp; ++k) {
                power *= p;
                term += power;
            }
            sigma_i *= term;

            tau_i   *= (exp + 1);
            omega_i += 1;

            phi_i = phi_i / p * (p - 1);
        }

        sigma_arr[i] = sigma_i;
        phi_arr[i]   = phi_i;
        tau_arr[i]   = tau_i;
        omega_arr[i] = omega_i;
    }
}

/* Build the list of terms up to MAX_LIMIT */
static void init_terms(idx_t limit)
{
    terms = (seq_t*)malloc((limit / 4 + 10) * sizeof(seq_t));
    terms_count = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t left = sigma_arr[i] - phi_arr[i];
        seq_t prod = 1;

        /* compute tau(i)^omega(i) while comparing with left */
        uint32_t times = (uint32_t)omega_arr[i];
        uint32_t k;
        for (k = 0; k < times; ++k) {
            prod *= tau_arr[i];
            if (prod > left) break;
        }

        if (prod > left) { /* inequality holds */
            terms[terms_count++] = (seq_t)i;
        }
    }
}

/* Ensures static data is initialized once */
static void ensure_initialized(void)
{
    static int initialized = 0;
    if (!initialized) {
        init_sieve(MAX_LIMIT);
        init_terms(MAX_LIMIT);
        initialized = 1;
    }
}

seq_t a(idx_t n)
{
    ensure_initialized();

    /* n is 1‑based: a(1) is first term (value 1) */
    return terms[n - 1];
}
