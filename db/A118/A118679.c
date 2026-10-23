/*
 * A118679  Absolute value of numerator of determinant of n X n matrix with
 *          M(i,j) = i/(i+1) if i=j otherwise 1.
 * https://oeis.org/A118679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = Numerator of (n^2 + 3*n - 2) / (2*(n+1)!)
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The numerator is obtained by cancelling the prime factors of
   the denominator from the numerator. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* maximum prime needed when n ≤ 10000      */
/* sqrt(max n^2+3n-2) ≈ 10000 , we sieve a bit higher */
#define PRIME_SIEVE_LIMIT 20000
#define MAX_PRIMES 2266  /* number of primes ≤ 20000 */

static bool primes_initialized = false;
static int primes[MAX_PRIMES];
static size_t primes_count = 0;

/* simple sieve of Eratosthenes */
static void init_primes(void)
{
    if (primes_initialized) return;

    bool is_comp[PRIME_SIEVE_LIMIT + 1] = { false };
    for (int i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[primes_count++] = i;
            if ((long long)i * i <= PRIME_SIEVE_LIMIT) {
                for (int j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    primes_initialized = true;
}

/* the required sequence function */
seq_t a(idx_t n)
{
    init_primes();

    /* compute n^2 + 3*n - 2 as a 64‑bit integer
       (this value is small enough for all n that are tested) */
    uint64_t n64 = (uint64_t)n;
    uint64_t N64 = n64 * n64 + 3 * n64 - 2;

    seq_t result = (seq_t)1;          /* final numerator after cancellation */

    uint64_t temp = N64;              /* factorise the numerator */

    /* factorise using the precomputed primes */
    for (size_t i = 0; i < primes_count &&
                     (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        int p = primes[i];
        if (temp % p != 0) continue;

        int eN = 0;          /* exponent of p in N64 */
        while (temp % p == 0) {
            temp /= p;
            ++eN;
        }

        /* exponent of p in denominator: 2*(n+1)! */
        int eD = 0;
        idx_t m = n + 1;     /* n+1 fits in idx_t (uint64_t) */
        while (m) {
            m /= p;
            eD += (int)m;
        }
        if (p == 2) eD += 1; /* extra factor 2 */

        int eKeep = eN - eD;   /* how many times p remains in the numerator */
        if (eKeep > 0) {
            for (int k = 0; k < eKeep; ++k)
                result *= (seq_t)p;
        }
    }

    /* any remaining prime factor > sqrt(N64) is prime itself */
    if (temp > 1) {
        int p = (int)temp;
        int eN = 1;

        int eD = 0;
        idx_t m = n + 1;
        while (m) {
            m /= p;
            eD += (int)m;
        }
        if (p == 2) eD += 1;

        int eKeep = eN - eD;
        if (eKeep > 0)
            result *= (seq_t)p;
    }

    return result;
}
