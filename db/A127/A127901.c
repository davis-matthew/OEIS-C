/*
 * A127901  Denominators of convergents to 6/Pi^2 based on 1/Zeta(s) =
 *          Sum_{k>=1} (mu(k)/k^s).
 * https://oeis.org/A127901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the n-th partial sum of Σ μ(k)/k² over the
 *            positive integers with μ(k)≠0. The partial sums are a(n) =
 *            Σ_{k=1}^{k_n} μ(k)/k², where k_n is the n‑th squarefree integer.
 *            The denominator of that reduced fraction is returned
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b) */

/* ------------------------------------------------------------------
   Prime sieve ------------------------------------------------------ */
#define MAX_PRIME_LIMIT 200000
static bool prime_flag[MAX_PRIME_LIMIT + 1];
static uint32_t primes[MAX_PRIME_LIMIT];
static size_t prime_count = 0;
static bool primes_initialised = false;

static void init_primes(void)
{
    if (primes_initialised) return;
    prime_flag[0] = prime_flag[1] = false;
    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i) prime_flag[i] = true;
    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (prime_flag[i]) {
            primes[prime_count++] = (uint32_t)i;
            if ((uint64_t)i * (uint64_t)i <= MAX_PRIME_LIMIT) {
                for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    prime_flag[j] = false;
            }
        }
    }
    primes_initialised = true;
}

/* ------------------------------------------------------------------
   Möbius function --------------------------------------------------- */
static int mobius(idx_t n)
{
    if (n == 1) return 1;
    int mu = 1;
    for (size_t i = 0; i < prime_count &&
         (idx_t)primes[i] * (idx_t)primes[i] <= n; ++i) {
        idx_t p = (idx_t)primes[i];
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
                if (exp > 1) return 0;           /* square factor */
            }
            mu = -mu;
        }
    }
    if (n > 1) mu = -mu;   /* remaining prime factor */
    return mu;
}

/* ------------------------------------------------------------------
   State for memoisation ------------------------------------------- */
static seq_t *den_cache = NULL;          /* cached denominators */
static idx_t cache_len  = 0;             /* how many cached */
static size_t cache_cap = 0;             /* allocated capacity */

static idx_t last_k      = 0;            /* last processed k */
static seq_t numer       = 0;            /* current numerator   */
static seq_t denom       = 1;            /* current denominator */

/* ------------------------------------------------------------------
   Main function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for out of range */

    init_primes();                       /* ensure primes ready   */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Find next k with μ(k) ≠ 0 */
        idx_t k = ++last_k;
        int mu = mobius(k);
        while (mu == 0) {
            k = ++last_k;
            mu = mobius(k);
        }
        /* k is now the square‑free index */
        /* k² as seq_t */
        seq_t ksq = (seq_t)k * (seq_t)k;

        /* lcm(denom, ksq) : store l in seq_t */
        seq_t g1 = gcd(denom, ksq);
        seq_t mul1 = ksq / g1;            /* ksq / g1 is exact integer   */
        seq_t part1 = numer * mul1;
        seq_t part2 = denom / g1;         /* μ‑term scaled by denom/g1  */
        seq_t new_num;
        if (mu > 0)
            new_num = part1 + part2;
        else
            new_num = part1 - part2;

        seq_t l = (denom / g1) * ksq;      /* lcm */
        seq_t g2 = gcd(new_num, l);
        numer = new_num / g2;
        denom = l / g2;

        /* Store the new denominator in the cache */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0 ? 16 : cache_cap << 1);
            seq_t *tmp = (seq_t *)realloc(den_cache,
                                          new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed – abort gracefully */
                return 0;
            }
            den_cache = tmp;
            cache_cap = new_cap;
        }
        den_cache[cache_len++] = denom;
    }

    return den_cache[n - 1];
}
