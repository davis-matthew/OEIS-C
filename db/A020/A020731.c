/*
 * A020731  Numbers n for which number of distinct prime divisors of C(n,k)
 *          has maximum at k = [n/2].
 * https://oeis.org/A020731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer N such that the number of
 *            distinct prime divisors of all binomial coefficients C(N,k)
 *            achieves its maximum at k=floor(N/2)
 * Range:     u8 n <= 116, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

/* --- static data for sieving --- */
#define MAX_N 200000            /* upper bound for sieve and computation */
#define MAX_PRIMES 120000       /* enough for all primes up to MAX_N */

static int spf[MAX_N + 1];       /* smallest prime factor */
static int prime_index[MAX_N + 1]; /* index of prime in primes[] */
static int primes[MAX_PRIMES];   /* list of primes */
static size_t prime_cnt = 0;
static bool sieve_done = false;

/* linear sieve for primes and spf */
static void do_sieve(void)
{
    if (sieve_done) return;
    sieve_done = true;
    memset(spf, 0, sizeof(spf));
    memset(prime_index, -1, sizeof(prime_index));
    for (int i = 2; i <= MAX_N; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = i;
            prime_index[i] = prime_cnt;
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt && primes[j] <= spf[i]
                && (long long)i * primes[j] <= MAX_N; ++j) {
            int p = primes[j];
            spf[i * p] = p;
        }
    }
}

/* update exponents for the factor 'num' with sign = +1 for multiply,
   -1 for divide.  'distinct' keeps the current number of distinct
   primes in the binomial coefficient. */
static void factor_update(idx_t num, int sign,
                          int *distinct, int *expC)
{
    while (num > 1) {
        int p = spf[(int)num];
        int cnt = 0;
        while (num % p == 0) {
            num /= p;
            ++cnt;
        }
        int pi = prime_index[p];
        int old = expC[pi];
        int neu = old + sign * cnt;
        if (old == 0 && neu > 0) ++(*distinct);
        else if (old > 0 && neu == 0) --(*distinct);
        expC[pi] = neu;
    }
}

/* test whether N satisfies the property */
static bool good_value(idx_t N)
{
    if (N == 0) return false;
    /* expC array holds exponents of all primes in current C(N,k) */
    static int expC[MAX_PRIMES];
    /* zero the used part */
    memset(expC, 0, prime_cnt * sizeof(int));

    int distinct = 0;          /* current number of distinct primes in C(N,k) */
    int max_distinct = 0;      /* maximum over all k */
    int mid_distinct = 0;      /* distinct at k = N/2 (floor) */

    idx_t mid = N / 2;
    for (idx_t k = 1; k <= N; ++k) {
        factor_update((idx_t)(N - k + 1), +1, &distinct, expC);  /* multiply numerator */
        factor_update((idx_t)k, -1, &distinct, expC);            /* divide denominator */

        if (distinct > max_distinct) max_distinct = distinct;
        if (k == mid) mid_distinct = distinct;
    }
    return mid_distinct == max_distinct;
}

/* cache of computed sequence terms */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;     /* number of terms stored (>=0) */
static size_t seq_cap = 0;     /* allocated capacity */
static idx_t tested_n = 0;     /* highest N that has been tested */

/* ensure cache has at least n terms */
static void ensure_terms(idx_t n)
{
    while (seq_len < n) {
        ++tested_n;
        if (good_value(tested_n)) {
            /* grow cache if needed */
            if (seq_len + 1 > seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)tested_n;
        }
    }
}

/* ------------------------------------------------------------ */
/*  Public entry point required by the harness                 */
seq_t a(idx_t n)
{
    assert(n >= 1);
    do_sieve();                 /* make sure primes are ready */
    ensure_terms(n);            /* fill cache up to required index */
    return seq_cache[n - 1];
}
