/*
 * A087812  Sum of MoebiusMu for numbers between n and 2n inclusive.
 * https://oeis.org/A087812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = sum_{k=n}^{2n} μ(k) (μ = Möbius function)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Constants for a modest upper bound that covers all needed primes. */
#define MAX_PRIME 200000

static idx_t   primes[MAX_PRIME];          /* prime list */
static size_t  primes_cnt   = 0;           /* number of primes generated   */
static bool    primes_ready = false;      /* one‑time initialization flag */
static bool    is_comp[MAX_PRIME + 1] = {0}; /* simple boolean sieve */

/* ----- One‑time prime generation (linear sieve) ----- */
static void init_primes(void)
{
    if (primes_ready) return;

    for (idx_t i = 2; i <= MAX_PRIME; ++i)
    {
        if (!is_comp[i])          /* i is prime */
            primes[primes_cnt++] = i;

        for (size_t j = 0; j < primes_cnt &&
            i * primes[j] <= MAX_PRIME; ++j)
        {
            is_comp[i * primes[j]] = true;
            if (i % primes[j] == 0) break;
        }
    }
    primes_ready = true;
}

/* ----- Möbius function μ(n) using trial division with the precomputed primes ----- */
static seq_t mobius(idx_t n)
{
    if (n == 1) return 1;    /* μ(1) = 1 */

    seq_t sign = 1;
    idx_t temp = n;

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i)
    {
        idx_t p = primes[i];
        if (temp % p == 0)
        {
            temp /= p;
            if (temp % p == 0)   /* p^2 divides n → μ(n)=0 */
                return 0;
            sign = -sign;
        }
    }
    if (temp > 1) sign = -sign;   /* remaining prime factor */
    return sign;
}

/* ----- Public function required by the build ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safely return 0 */

    init_primes();                 /* ensure the prime table exists */

    seq_t sum = 0;
    for (idx_t k = n; k <= 2 * n; ++k)
        sum += mobius(k);

    return sum;
}
