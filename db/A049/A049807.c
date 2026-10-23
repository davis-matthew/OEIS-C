/*
 * A049807  a(n)=number of Farey fractions of order n that are <=1/3; cf.
 *          A049805.
 * https://oeis.org/A049807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{b=2..n} #{a | 1 ≤ a ≤ ⌊b/3⌋ , gcd(a,b)=1}
 * Range:     u8 n <= 49, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (= number of Farey fractions of order n that are ≤ 1/3) */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Static storage for the cache and smallest‑prime‑factor sieve     */
/* ------------------------------------------------------------------ */
static seq_t   *cache   = NULL;   /* cumulative a(1)..a(n) */
static idx_t    cache_len  = 0;   /* how many terms are cached */
static idx_t    cache_cap  = 0;   /* allocated capacity of cache   */

static idx_t   *spf     = NULL;   /* smallest prime factor table  */
static idx_t    spf_cap = 0;      /* allocated size of spf table   */

/* ------------------------------------------------------------------ */
/*  Compute (or extend) the cache up to the required index.          */
/* ------------------------------------------------------------------ */
static void precompute(idx_t target)
{
    /* ---------- ensure array sizes ---------- */
    if (cache_cap < target + 1) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (target + 1));
        if (!new_cache) exit(EXIT_FAILURE);
        cache       = new_cache;
        cache_cap   = target + 1;
    }

    if (spf_cap < target + 1) {
        idx_t *new_spf = (idx_t *)realloc(spf, sizeof(idx_t) * (target + 1));
        if (!new_spf) exit(EXIT_FAILURE);
        spf        = new_spf;
        spf_cap    = target + 1;
    }

    /* ---------- build smallest‑prime‑factor sieve ---------- */
    for (idx_t i = 0; i <= target; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= target; ++i) {
        if (spf[i] == i) {                    /* i is prime */
            for (idx_t j = i * i; j <= target; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    /* ---------- extend the cache ---------- */
    seq_t sum = (cache_len > 0) ? cache[cache_len-1] : 0;

    for (idx_t b = cache_len + 1; b <= target; ++b) {
        seq_t add = 0;

        if (b == 1) {
            add = 1;                        /* only 0/1 */
        } else {
            idx_t m = b / 3;                /* floor(b/3) */
            if (m > 0) {
                /* collect distinct prime factors of b */
                idx_t primes[16];
                int    pcount = 0;
                idx_t temp = b;
                while (temp > 1) {
                    idx_t p = spf[temp];
                    primes[pcount++] = p;
                    while (temp % p == 0) temp /= p;
                }

                /* inclusion–exclusion */
                seq_t res = (seq_t)m;
                int subsets = 1 << pcount;
                for (int mask = 1; mask < subsets; ++mask) {
                    idx_t mult = 1;
                    int    bits = 0;
                    for (int i = 0; i < pcount; ++i)
                        if (mask & (1 << i)) {
                            mult *= primes[i];
                            ++bits;
                        }

                    seq_t term = (seq_t)(m / mult);
                    if (bits & 1) res -= term;   /* odd number of primes */
                    else          res += term;   /* even number of primes */
                }
                add = res;
            }
        }

        sum += add;
        cache[b-1] = sum;
    }

    cache_len = target;
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* undefined for n < 1 */

    /* First call: initialise storage */
    if (cache == NULL) {
        cache_cap = 64;
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(EXIT_FAILURE);
        cache_len = 0;
        spf_cap   = 0;
        spf       = NULL;
    }

    if (n > cache_len) precompute(n);
    return cache[n-1];
}
