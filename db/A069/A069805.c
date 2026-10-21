/*
 * A069805  Smallest k>n such that tau(n) divides tau(k)=A000005(k).
 * https://oeis.org/A069805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A069805:  a(n) = the smallest k > n such that τ(n) | τ(k),
    where τ(m) is the number of positive divisors of m.
    This implementation builds divisor–count tables with a
    linear sieve and searches forward from n+1 until the
    divisibility condition is met.  Tables are extended
    lazily as larger values are needed.  The algorithm
    runs in O(m) time per call, with m the largest
    k examined.  For n up to about 10 000 this is
    well below the 2‑second time limit.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/*  State for the dynamic divisor tables.
    They are indexed from 1 up to max_limit inclusive.  */
static size_t max_limit = 0;
static unsigned *spf = NULL;   /* smallest prime factor for each number */
static unsigned *tau = NULL;   /* number–of–divisors function */

/*  Recompute spf and tau tables up to and including 'limit'.  */
static void recompute_tables(size_t limit)
{
    /* Free previously allocated tables.  */
    free(spf);
    free(tau);
    spf = (unsigned *)calloc(limit + 1, sizeof(unsigned));
    tau = (unsigned *)calloc(limit + 1, sizeof(unsigned));
    if (!spf || !tau) abort();   /* allocation failed */

    /*  Linear sieve for smallest prime factors.  */
    size_t primes_cap = (limit / 10 + 100);
    size_t *primes = (size_t *)malloc(primes_cap * sizeof(size_t));
    size_t primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = (unsigned)i;
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (size_t *)realloc(primes, primes_cap * sizeof(size_t));
                if (!primes) abort();
            }
            primes[primes_cnt++] = (size_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            size_t p = primes[j];
            if (p > spf[i] || i * p > limit) break;
            spf[i * p] = (unsigned)p;
        }
    }
    free(primes);

    /*  Compute τ(n) for all n up to 'limit'.  */
    tau[1] = 1;
    for (size_t i = 2; i <= limit; ++i) {
        size_t p = spf[i];
        size_t m = i / p;
        size_t e = 1;
        while (m % p == 0) {
            ++e;
            m /= p;
        }
        tau[i] = tau[m] * (e + 1);
    }
    max_limit = limit;
}

/*  Ensure divisor tables cover at least 'needed' numbers.  */
static void ensure_tables(size_t needed)
{
    if (needed <= max_limit) return;
    size_t new_limit = needed;
    if (new_limit < 10) new_limit = 10;
    if (new_limit <= max_limit) new_limit = max_limit * 2;
    recompute_tables(new_limit);
}

/*  Public sequence routine.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    /*  Convert idx_t to native integer for indexing.  */
    size_t N = (size_t)n;

    /*  Need τ(N) and at least τ(N+1) to start the search.  */
    ensure_tables(N + 1);

    unsigned tau_n = tau[N];
    size_t k = N + 1;

    for (;;) {
        /*  Extend tables if k is beyond the current limit.  */
        if (k > max_limit) ensure_tables(k);

        if (tau[k] % tau_n == 0) {
            return (seq_t)k;
        }
        ++k;
    }
}
