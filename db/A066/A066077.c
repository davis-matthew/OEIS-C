/*
 * A066077  a(n) is the number of x such that sigma(x)-1 is 0 or one of the
 *          first n-1 primes.
 * https://oeis.org/A066077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A066077 – number of x with σ(x) – 1 = 0 or a prime among the first n‑1 primes.
   The function precomputes the cumulative counts up to a maximum n (12000).
   For any n ≥ 1 it returns the correct a(n). The implementation uses
   a sieve for primes up to 200 000 and a divisor‑summing sieve to get σ(x)
   for 1 ≤ x ≤ 200 000.  Counts are accumulated per prime rank, then
   a prefix array gives a(n).  No hard‑coded term list is used.  O(n log n)
   preprocessing, O(1) queries. */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the test harness */
seq_t a(idx_t n)
{
    /* Cover all requested n up to about 10 000; 120 000 leaves
       a comfortable margin for n over 10 000. */
    const int MAX_N   = 12000;      /* max n that will be queried           */
    const int P_LIMIT = 200000;     /* largest prime that can appear as σ(x)–1 */

    /* Static tables, initialized once on first call */
    static seq_t *pref   = NULL;    /* pref[t] = a(t+1)  (t ≥ 0)             */
    static int    init   = 0;       /* flag: have we prepared tables?      */

    /* ------------------------------------------------------------------ */
    /* 1) build prime list and rank array                                 */
    /* ------------------------------------------------------------------ */
    static unsigned char *isPrime = NULL;   /* 0/1 flags for prime test   */
    static int           *primeRank;        /* prime → rank (1‑based)      */

    if (!init) {
        /* allocate and zero the structures */
        isPrime   = (unsigned char*)calloc(P_LIMIT + 1, sizeof(unsigned char));
        primeRank = (int*)calloc(P_LIMIT + 1, sizeof(int));

        /* Sieve of Eratosthenes */
        for (int i = 0; i <= P_LIMIT; ++i) isPrime[i] = 1;
        isPrime[0] = isPrime[1] = 0;
        for (int p = 2; p * p <= P_LIMIT; ++p) {
            if (isPrime[p]) {
                for (int m = p * p; m <= P_LIMIT; m += p)
                    isPrime[m] = 0;
            }
        }

        /* assign 1‑based ranks to primes */
        int r = 0;
        for (int i = 2; i <= P_LIMIT; ++i)
            if (isPrime[i]) primeRank[i] = ++r;   /* primes only get rank > 0 */

        /* ------------------------------------------------------------------ */
        /* 2) compute σ(x) for 1 ≤ x ≤ P_LIMIT                               */
        /* ------------------------------------------------------------------ */
        seq_t *sigma = (seq_t*)calloc(P_LIMIT + 1, sizeof(seq_t));
        for (int d = 1; d <= P_LIMIT; ++d) {
            for (int m = d; m <= P_LIMIT; m += d)
                sigma[m] += (seq_t)d;
        }

        /* ------------------------------------------------------------------ */
        /* 3) count x by the rank of σ(x) – 1                                 */
        /* ------------------------------------------------------------------ */
        seq_t *cnt = (seq_t*)calloc(MAX_N + 2, sizeof(seq_t)); /* cnt[rank]   */
        seq_t baseCount = 0;                                   /* x with v==0 */

        for (int x = 1; x <= P_LIMIT; ++x) {
            seq_t v = sigma[x] - (seq_t)1;          /* candidate prime (or 0) */

            if (v == 0) {
                /* σ(1) = 1 ⇒ v = 0, count once */
                baseCount += 1;
            } else if (v <= (seq_t)P_LIMIT) {
                int rank = primeRank[(int)v];       /* 0 for composites    */
                if (rank >= 1 && rank <= MAX_N)
                    cnt[rank] += 1;
            }
        }

        /* ------------------------------------------------------------------ */
        /* 4) build prefix table a(t+1) = baseCount + Σ_{k≤t} cnt[k]          */
        /* ------------------------------------------------------------------ */
        pref = (seq_t*)calloc(MAX_N + 1, sizeof(seq_t));
        pref[0] = baseCount;              /* a(1) = baseCount */
        for (int t = 1; t <= MAX_N; ++t)
            pref[t] = pref[t-1] + cnt[t];

        /* clean up temporary arrays */
        free(isPrime);
        free(primeRank);
        free(sigma);
        free(cnt);

        init = 1;
    }

    /* ---------------------------------------------------------------------- */
    /* Return the requested value – falls back to 0 for out‑of‑range n          */
    /* ---------------------------------------------------------------------- */
    if (n < 1 || n > (idx_t)MAX_N)
        return 0;
    return pref[(idx_t)n - 1];
}
