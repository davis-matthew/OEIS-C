/*
 * A023832  Sum of exponents in prime-power factorization of C(4n,n+2).
 * https://oeis.org/A023832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of exponents in prime‑power factorization of C(4n,
 *            n+2)
 * Range:     u8 n <= 724, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is a(n) = Ω(C(4n, n+2)) = Σ_p (⌊4n/p⌋ + ⌊4n/p²⌋ + … – ⌊(n+2)/p⌋ – ⌊(n+2)/p²⌋ – … – ⌊(3n-2)/p⌋ – ⌊(3n-2)/p²⌋ – …) */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/* static data for prime generation                                    */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL;    /* array of primes */
static size_t   prime_cnt   = 0;        /* number of primes stored */
static idx_t    prime_limit = 0;       /* largest prime precomputed   */
static unsigned char *sieve  = NULL;   /* temporary sieve              */
static idx_t    sieve_limit = 0;       /* size of sieve               */

/* ------------------------------------------------------------------ */
/* ensure primes up to 'limit' are available in prime_list            */
/* ------------------------------------------------------------------ */
static void
ensure_primes (idx_t limit)
{
    if (limit <= prime_limit) return;   /* already enough */

    /* free previous sieve if any */
    if (sieve) free (sieve);
    sieve_limit = limit;
    sieve = (unsigned char *)calloc (sieve_limit + 1, 1);   /* 0 -> prime */
    if (!sieve) return;                                 /* allocation failure */

    sieve[0] = sieve[1] = 1;                          /* not prime */

    idx_t sq = (idx_t)floor (sqrt ((double)limit));
    for (idx_t i = 2; i <= sq; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) sieve[j] = 1;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (!sieve[i]) ++cnt;

    /* allocate and fill prime list */
    uint64_t *new_list = (uint64_t *)malloc (cnt * sizeof (uint64_t));
    if (!new_list) return;                     /* allocation failure */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i]) new_list[idx++] = i;

    /* replace old list */
    free (prime_list);
    prime_list = new_list;
    prime_cnt  = cnt;
    prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t
a (idx_t n)
{
    if (n < 1) return 0;           /* offset starts at 1 */

    /* prepare primes up to 4*n */
    idx_t lim = 4 * n;
    ensure_primes (lim);

    seq_t ans = 0;

    /* for each prime use Legendre's formula to add exponent */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = prime_list[pi];
        if (p > lim) break;

        seq_t e = 0;

        idx_t t = lim;
        while (t) { t /= p; e += (seq_t)t; }

        t = n + 2;
        while (t) { t /= p; e -= (seq_t)t; }

        idx_t t2 = 3 * n - 2;
        while (t2) { t2 /= p; e -= (seq_t)t2; }

        ans += e;
    }

    return ans;
}
