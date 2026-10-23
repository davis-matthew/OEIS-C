/*
 * A023819  Sum of exponents in prime-power factorization of C(3n,n).
 * https://oeis.org/A023819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A022559(3*n) - A022559(2*n) - A022559(n) i.e. sum of
 *            exponents in the prime‑power factorisation of C(3n,n)
 * Range:     u8 n <= 858, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* -------------------------------------------------------------------- */
/* Prime sieve helpers                                                */
/* -------------------------------------------------------------------- */

static uint64_t *prime_list = NULL;     /* stored primes   */
static size_t   prime_cnt   = 0;        /* number of primes */
static idx_t    prime_limit = 0;        /* current sieve limit */

/* Ensure prime_list contains all primes ≤ limit.  Recomputes the list
   whenever limit > prime_limit.  This routine is safe in C++ because
   the void* from calloc is explicitly cast. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* discard old list */
    free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;
    prime_limit = limit;

    unsigned char *sieve = (unsigned char *)calloc(limit + 1, 1);
    if (!sieve) return;               /* allocation failure: leave list empty */

    idx_t i, j;
    for (i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    /* count primes to allocate memory */
    for (i = 2; i <= limit; ++i) if (!sieve[i]) ++prime_cnt;

    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) { free(sieve); return; }

    size_t k = 0;
    for (i = 2; i <= limit; ++i)
        if (!sieve[i]) prime_list[k++] = i;

    free(sieve);
}

/* -------------------------------------------------------------------- */
/* Main sequence function                                              */
/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) = 0 by definition */

    idx_t limit3 = 3 * n;
    idx_t limit2 = 2 * n;

    ensure_primes(limit3);                /* must have primes up to 3n */

    seq_t result = 0;

    /* Sum over primes ≤ 3n the exponents of each prime in C(3n,n) */
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        idx_t p = (idx_t)prime_list[idx];
        if (p > limit3) break;

        idx_t power = p;
        while (power <= limit3) {
            idx_t term1 = limit3 / power;
            idx_t term2 = limit2 / power;
            idx_t term3 = n / power;
            result += (seq_t)(term1 - term2 - term3);

            /* advance to next power of p, stopping before overflow */
            if (power > limit3 / p) break;
            power *= p;
        }
    }

    return result;
}
