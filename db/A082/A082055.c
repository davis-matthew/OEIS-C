/*
 * A082055  Product of common prime-divisors (without multiplicity) of
 *          sigma(n) and phi(n).
 * https://oeis.org/A082055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = squarefree kernel of gcd(σ(n), φ(n))
 * Range:     u8 n <= 585, u16 n <= 9902, u32 n <= 9902, u64 n <= 9902,
 *            u128 n <= 9902, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here σ(n) is the sum of divisors, φ(n) is Euler's totient.
   We compute σ and φ by factoring n, obtain their gcd,
   and return the product of the distinct prime factors of that gcd. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime sieve (once per program) ---------- */
static bool primes_initialized = false;
static int *primes_arr = NULL;
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_initialized)
        return;

    const size_t limit = 1000000; /* enough for n up to about 10⁶ */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) exit(1); /* allocation failure */

    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Count primes */
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++primes_cnt;

    primes_arr = (int *)malloc(primes_cnt * sizeof(int));
    if (!primes_arr) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes_arr[idx++] = (int)i;

    free(is_prime);
    primes_initialized = true;
}

/* ---------- Greatest common divisor for seq_t ---------- */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1 */

    init_primes();

    idx_t temp = n;
    seq_t phi = n;     /* φ(n) starts as n */
    seq_t sigma = 1;   /* σ(n) starts as 1 */

    /* Factor n using small primes */
    for (size_t i = 0; i < primes_cnt; ++i) {
        int p = primes_arr[i];
        idx_t p_idx = (idx_t)p;
        if (p_idx * p_idx > temp)
            break;

        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* Update φ(n) : multiply by (p-1)/p */
            phi = phi / p * ((seq_t)(p - 1));

            /* Update σ(n) : multiply by (1 + p + … + p^e) */
            seq_t term = 1;
            seq_t power = 1;
            for (int j = 1; j <= e; ++j) {
                power *= (seq_t)p;
                term += power;
            }
            sigma *= term;
        }
    }

    if (temp > 1) { /* remaining prime factor > 1 */
        idx_t p = temp;
        phi = phi / p * ((seq_t)(p - 1));
        sigma *= ((seq_t)(1 + p));
    }

    /* Compute squarefree kernel of gcd(σ, φ) */
    seq_t g = seq_gcd(sigma, phi);
    seq_t res = 1;
    seq_t g_tmp = g;

    /* Factor g using the same prime list */
    for (size_t i = 0; i < primes_cnt; ++i) {
        int p = primes_arr[i];
        seq_t p_t = (seq_t)p;
        if (p_t * p_t > g_tmp)
            break;
        if (g_tmp % p == 0) {
            res *= p_t;
            while (g_tmp % p == 0)
                g_tmp /= p;
        }
    }

    if (g_tmp > 1) /* g_tmp itself is a prime factor > sqrt(original g) */
        res *= g_tmp;

    return res;
}
