/*
 * A066695  Numbers k such that Euler phi(k) / Carmichael lambda(k) = 12.
 * https://oeis.org/A066695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3734, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of k such that φ(k) / λ(k) = 12.
   φ is Euler's totient; λ is Carmichael's function. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

/* ---------- Prime list (dynamic linear sieve) ---------- */
static uint32_t *primes = NULL;      /* array of primes */
static size_t primes_size = 0;
static size_t primes_cap  = 0;
static uint32_t prime_max = 1;       /* primes are known up to prime_max */

static void extend_primes(uint32_t new_max)
{
    if (new_max <= prime_max) return;

    /* simple sieve up to new_max */
    uint32_t *mark = (uint32_t *)calloc(new_max + 1, sizeof(uint32_t));
    if (!mark) exit(1);

    for (uint32_t i = 2; i * i <= new_max; ++i)
        if (!mark[i])
            for (uint32_t j = i * i; j <= new_max; j += i)
                mark[j] = 1;

    for (uint32_t i = 2; i <= new_max; ++i)
        if (!mark[i]) {
            if (primes_size >= primes_cap) {
                size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_size++] = i;
        }

    free(mark);
    prime_max = new_max;
}

/* ---------- GCD for seq_t ---------- */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---------- Test whether phi(n)/lambda(n) == 12 ---------- */
static int good(uint64_t n)
{
    uint64_t m = n;
    seq_t phi   = (seq_t)1;
    seq_t lcm   = (seq_t)1;

    /* factorization by trial division */
    uint32_t sqrt_n = (uint32_t)(sqrt((double)n) + 1.0);
    extend_primes(sqrt_n);

    for (size_t i = 0; i < primes_size && (uint64_t)primes[i] * (uint64_t)primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p != 0) continue;
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        /* φ(p^exp) = p^(exp-1) * (p-1) */
        seq_t p_pow = (seq_t)1;
        for (int j = 0; j < exp - 1; ++j) p_pow *= (seq_t)p;
        seq_t phi_contrib = p_pow * (seq_t)(p - 1);
        phi *= phi_contrib;

        /* λ(p^exp) */
        seq_t lam;
        if (p == 2) {
            if (exp == 1) lam = (seq_t)1;
            else if (exp == 2) lam = (seq_t)2;
            else {
                lam = (seq_t)1;
                for (int j = 0; j < exp - 2; ++j) lam *= (seq_t)2;
            }
        } else {
            lam = phi_contrib;
        }

        /* lcm update */
        seq_t g = gcd_seq_t(lcm, lam);
        lcm = (lcm / g) * lam;
    }

    /* remaining prime factor > sqrt(n) */
    if (m > 1) {
        uint32_t p = (uint32_t)m;
        int exp = 1;
        seq_t p_pow = (seq_t)1;            /* p^(exp-1) = 1 */
        seq_t phi_contrib = p_pow * (seq_t)(p - 1);
        phi *= phi_contrib;

        seq_t lam = phi_contrib;           /* λ(p) = p-1 */
        seq_t g = gcd_seq_t(lcm, lam);
        lcm = (lcm / g) * lam;
    }

    if (phi % lcm != (seq_t)0) return 0;
    return (phi / lcm) == (seq_t)12;
}

/* ---------- Storage of found terms ---------- */
static seq_t *values   = NULL;
static size_t values_size = 0;
static size_t values_cap  = 0;

static void push_value(seq_t val)
{
    if (values_size >= values_cap) {
        size_t new_cap = (values_cap == 0) ? 16 : values_cap * 2;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        values = tmp;
        values_cap = new_cap;
    }
    values[values_size++] = val;
}

/* ---------- Main sequence function ---------- */
static uint64_t last_tested = 1;   /* last integer checked */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset 1 */

    /* generate terms until we have at least n */
    while (values_size < (size_t)n) {
        ++last_tested;

        /* ensure primes up to sqrt(last_tested) are known */
        uint32_t need = (uint32_t)(sqrt((double)last_tested) + 1.0);
        extend_primes(need);

        if (good(last_tested))
            push_value((seq_t)last_tested);
    }
    return values[(size_t)n - 1];
}
