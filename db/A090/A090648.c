/*
 * A090648  a(n)=2*(4^n-1)/denominator(B(2n)) where B(k) denotes the k-th
 *          Bernoulli number.
 * https://oeis.org/A090648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(4^n - 1) / ∏{p prime | p-1 | 2n} (denominator of
 *            Bernoulli B(2n))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* ---------- helper to compute 4^n as seq_t ---------- */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ---------- sieve of primes up to 20001 (covers 2n+1 for n ≤ 10000) ---------- */
#define PRIME_SIEVE_MAX 20001
static int primes[PRIME_SIEVE_MAX + 1];
static int prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    bool comp[PRIME_SIEVE_MAX + 1] = {0};
    for (int i = 2; i <= PRIME_SIEVE_MAX; ++i)
    {
        if (!comp[i])
        {
            primes[prime_cnt++] = i;
            if ((long long)i * i <= PRIME_SIEVE_MAX)
            {
                for (int j = i * i; j <= PRIME_SIEVE_MAX; j += i)
                    comp[j] = true;
            }
        }
    }
    primes_initialized = true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    init_primes();

    /* compute 4^n */
    seq_t pow4 = pow_seq_t((seq_t)4, n);

    /* numerator = 2 * (4^n - 1) */
    seq_t numerator = (seq_t)2 * (pow4 - (seq_t)1);

    /* compute denominator = product of primes p with (p-1) | 2n */
    unsigned long long two_n = (unsigned long long)n * 2ULL;
    seq_t denom = (seq_t)1;
    for (int i = 0; i < prime_cnt; ++i)
    {
        int p = primes[i];
        if ((unsigned long long)p > two_n + 1ULL) break;
        if ((two_n % (unsigned long long)(p - 1)) == 0ULL)
            denom *= (seq_t)p;
    }

    return numerator / denom;
}
