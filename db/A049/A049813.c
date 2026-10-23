/*
 * A049813  a(n)=number of Farey fractions of order n that are <=1/9; cf.
 *          A049805.
 * https://oeis.org/A049813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{b=2}^{n} #{a: 1≤a≤⌊b/9⌋ , gcd(a,b)=1}
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t *spf = NULL;       /* smallest prime factor array */
static idx_t spf_lim = 0;

static seq_t *cache = NULL;     /* cached sequence values, 1‑based */
static idx_t cache_max = 0;     /* highest index computed */

/* Build smallest prime factor array up to LIMIT (inclusive) */
static void build_spf(idx_t LIMIT)
{
    free(spf);
    spf = (idx_t *)malloc((LIMIT + 1) * sizeof(idx_t));
    if (!spf) return;           /* out of memory – caller will fail */
    spf[0] = 0;
    if (LIMIT >= 1) spf[1] = 1;
    for (idx_t i = 2; i <= LIMIT; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) if (!spf[i]) {
        spf[i] = i;
        if (i <= LIMIT / i) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                if (!spf[j]) spf[j] = i;
        }
    }
    spf_lim = LIMIT;
}

/* Ensure spf array covers up to N */
static void ensure_spf(idx_t N)
{
    if (N <= spf_lim) return;
    build_spf(N);
}

/* Count integers a in [1..K] coprime to N using inclusion–exclusion */
static seq_t coprime_upto(idx_t N, idx_t K)
{
    if (K == 0) return 0;
    /* gather distinct prime factors of N */
    idx_t primes[16];
    int pc = 0;
    while (N > 1) {
        idx_t p = spf[N];
        primes[pc++] = p;
        while (N % p == 0) N /= p;
    }
    seq_t result = (seq_t)K;
    int subsets = 1 << pc;
    for (int mask = 1; mask < subsets; ++mask) {
        idx_t prod = 1;
        int bits = 0;
        for (int i = 0; i < pc; ++i) if (mask & (1 << i)) {
            prod *= primes[i];
            ++bits;
            if (prod > K) break;
        }
        if (prod > K) continue;
        seq_t t = (seq_t)(K / prod);
        if (bits & 1) result -= t;   /* odd number of primes – subtract */
        else          result += t;   /* even number – add back */
    }
    return result;
}

/* Ensure cache covered up to N and compute missing terms */
static void ensure_cache(idx_t N)
{
    if (N <= cache_max) return;
    if (!cache) {
        cache = (seq_t *)malloc((N + 1) * sizeof(seq_t));
        if (!cache) return;         /* memory error – caller will return 0 */
        cache[0] = 0;
    } else {
        cache = (seq_t *)realloc(cache, (N + 1) * sizeof(seq_t));
        if (!cache) return;         /* memory error */
    }
    /* compute values from cache_max+1 up to N */
    for (idx_t i = cache_max + 1; i <= N; ++i) {
        if (i == 1) {
            cache[i] = 1;          /* only 0/1 satisfies ≤1/9 */
        } else {
            idx_t limit = i / 9;
            seq_t add = coprime_upto(i, limit);
            cache[i] = cache[i - 1] + add;
        }
    }
    cache_max = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_spf(n);
    ensure_cache(n);
    return cache[n];
}
