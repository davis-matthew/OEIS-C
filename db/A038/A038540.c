/*
 * A038540  a(n) = number of modules with n elements over the ring of integers
 *          in the real quadratic field of discriminant 40.
 * https://oeis.org/A038540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} g(p,e) where g(p,e) = partition(e) if (40/p)=0
 *            (ramified); g(p,e) = 0 if (40/p)=-1 and e odd (inert); g(p,e) =
 *            partition(e/2) if (40/p)=-1 and e even; g(p,e) = Σ_{k=0..e}
 *            partition(k)·partition(e−k) if (40/p)=1 (split)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   chi_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/chi_int.h"

/* estimate of primes needed to factor n≤10000 */
#define PRIME_LIMIT 20000
static idx_t primes[PRIME_LIMIT];
static size_t prime_cnt = 0;

static seq_t part[21];              /* partitions up to 20 (enough for e≤log₂10000≈14) */
static bool globals_ini = false;

/* initialise primes and partition values */
static void init_globals(void)
{
    if (globals_ini) return;
    globals_ini = true;

    /* sieve primes up to PRIME_LIMIT */
    bool marked[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) marked[i] = false;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
    {
        if (!marked[i])
        {
            primes[prime_cnt++] = i;
            if ((idx_t)i * i <= PRIME_LIMIT)
            {
                for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                    marked[j] = true;
            }
        }
    }

    /* partition numbers */
    part[0] = (seq_t)1;
    for (int n = 1; n <= 20; ++n) part[n] = (seq_t)0;
    for (int n = 1; n <= 20; ++n)
        for (int k = n; k <= 20; ++k)
            part[k] = (seq_t)((seq_t)part[k] + (seq_t)part[k - n]);
}

static seq_t part_val(idx_t e)
{
    return part[(int)e];
}

/* value for a prime power p^e */
static seq_t a_prime_power(idx_t p, idx_t e)
{
    if (e == 0) return (seq_t)1;
    int ch = chi_int((int)p);
    if (ch == 0)                 /* ramified */
        return part_val(e);
    if (ch == 1)                 /* split */
    {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= e; ++k)
        {
            sum = (seq_t)(sum + part_val(k) * part_val(e - k));
        }
        return sum;
    }
    /* inert */
    if (e & 1U) return (seq_t)0;
    return part_val(e / 2);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but keep consistent */
    init_globals();

    seq_t result = (seq_t)1;
    idx_t m = n;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= m; ++i)
    {
        idx_t p = primes[i];
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            result = (seq_t)(result * a_prime_power(p, e));
        }
    }
    if (m > 1 /* remaining prime factor */)
        result = (seq_t)(result * a_prime_power(m, 1));

    return result;
}
