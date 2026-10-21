/*
 * A051864  Sum of transposition distances (divided by 2) present in the
 *          permutation produced by inverses of 1..(p-1) computed in Zp, where
 *          p is n-th prime.
 * https://oeis.org/A051864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the absolute differences |i – inv_p(i)| over i = 1
 *            … p–1, where inv_p(i) is the modular inverse of i modulo p,
 *            divided by 2. Here p is the n‑th prime. The implementation
 *            caches the result for each prime index
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/*  Constants ----------------------------------------------------------- */
#define MAX_PRIMES   12000     /* enough for n up to ≈ 10000               */
#define MAX_PRIME    130000    /* accommodates the 12000‑th prime          */

/* --------------------------------------------------------------------- */
/*  Global data --------------------------------------------------------- */
static idx_t primes[MAX_PRIMES];
static idx_t prime_cnt = 0;      /* number of primes stored                */

static seq_t cache[MAX_PRIMES];
static idx_t cache_sz = 0;       /* highest index filled in cache          */

static seq_t inv[MAX_PRIME];     /* temporary array to store inverses       */

/* --------------------------------------------------------------------- */
/*  Utility: sieve of Eratosthenes ------------------------------------- */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;

    static bool is_composite[MAX_PRIME + 1] = { false };
    for (idx_t i = 2; i <= MAX_PRIME && prime_cnt < MAX_PRIMES; ++i)
    {
        if (!is_composite[i])
        {
            primes[prime_cnt++] = i;
            if (i * i <= MAX_PRIME)
            {
                for (idx_t j = i * i; j <= MAX_PRIME; j += i)
                    is_composite[j] = true;
            }
        }
    }
    done = true;
}

/* --------------------------------------------------------------------- */
/*  Compute the n‑th term ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* below offset – undefined           */

    init_primes();                  /* primes[0 .. prime_cnt-1] ready   */

    /* Ensure we have at least n primes prepared.  The sieve above
       guarantees this for all n <= MAX_PRIMES.                 */
    if (prime_cnt < n)
        return 0;                   /* safety net – should not occur     */

    /* Compute terms whose values are not yet cached.           */
    for (idx_t k = cache_sz + 1; k <= n; ++k)
    {
        idx_t p = primes[k - 1];    /* p is the k‑th prime, 1‑based      */
        seq_t pp = (seq_t)p;

        /* ----- compute the inverse permutation modulo p ---- */
        inv[1] = 1;
        for (idx_t i = 2; i < p; ++i)
        {
            /* inv[i] = p - (p / i) * inv[p % i] mod p            */
            unsigned long long mul = (unsigned long long)(p / i)
                                     * (unsigned long long)inv[p % i];
            mul %= (unsigned long long)p;
            seq_t val = (seq_t)(( (unsigned long long)p - mul ) % (unsigned long long)p);
            if (val == pp) val = 0;    /* avoid the case when subtraction wraps   */
            inv[i] = val;
        }

        /* ----- accumulate half of the transposition distances ---- */
        seq_t half_sum = 0;
        for (idx_t i = 1; i < p; ++i)
        {
            if (inv[i] > (seq_t)i)
                half_sum += inv[i] - (seq_t)i;
        }

        cache[k - 1] = half_sum;      /* store the term */
    }

    cache_sz = n;
    return cache[n - 1];
}
