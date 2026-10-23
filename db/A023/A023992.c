/*
 * A023992  Sum of exponents of primes in multinomial coefficient M(4n;
 *          n+2,2n,n-2).
 * https://oeis.org/A023992
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A022559(4*n) - A022559(2*n) - A022559(n+2) - A022559(n-2)
 * Range:     u8 n <= 344, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (A022559(k) = ∑_{i=1}^k Ω(i), Ω being the number of prime factors of i with multiplicity) */

#include <stdint.h>
#include <stdlib.h>

/* static cache for primes and prefix sums of Ω */
static idx_t    cache_limit = 0;     /* maximum index for which cache is valid */
static uint32_t *spf   = NULL;       /* smallest prime factor for each number */
static uint8_t  *omega = NULL;       /* Ω(i) for each number (with multiplicity) */
static seq_t    *A022  = NULL;       /* prefix sums of Ω */

/*  Helper: build or extend cache up to 'limit' (inclusive). */
static void ensure_cache(idx_t limit)
{
    if (limit <= cache_limit)
        return;   /* already sufficient */

    /* Allocate new arrays (cast for C++). */
    uint32_t *new_spf   = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    uint8_t  *new_omega = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    seq_t    *new_A022  = (seq_t*)malloc((limit + 1) * sizeof(seq_t));

    if (!new_spf || !new_omega || !new_A022) {
        free(new_spf); free(new_omega); free(new_A022);
        abort();
    }

    /* Sieve for smallest prime factor (SPF). */
    for (idx_t i = 0; i <= limit; ++i)
        new_spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i <= limit / i) {                     /* avoid overflow of i*i */
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (new_spf[j] == 0)
                        new_spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* Compute Ω(i) for each i: Ω(1)=0, else Ω(i)=Ω(i/SPF[i]) + 1. */
    new_omega[0] = 0;
    new_omega[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        new_omega[i] = new_omega[i / new_spf[i]] + 1;
    }

    /* Prefix sums A022(k) = Σ_{i=1}^k Ω(i). */
    new_A022[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        new_A022[i] = new_A022[i - 1] + (seq_t)new_omega[i];
    }

    /* Replace old cache with new. */
    free(spf);  free(omega);  free(A022);
    spf   = new_spf;
    omega = new_omega;
    A022  = new_A022;
    cache_limit = limit;
}

/*  Return a(n) for n >= 2.  Uses the cached A022 values. */
seq_t a(idx_t n)
{
    /* Ensure cache covers up to 4*n. */
    idx_t k4 = 4 * n;
    ensure_cache(k4);

    /* For n=2, n-2 == 0; A022[0] is defined as 0. */
    idx_t km = (n >= 2) ? (n - 2) : 0;
    idx_t kp = n + 2;

    seq_t res = A022[k4];
    res -= A022[2 * n];
    res -= A022[kp];
    res -= A022[km];
    return res;
}
