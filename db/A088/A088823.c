/*
 * A088823  a(n) is the GCD of the sum of largest prime factors of numbers
 *          from 1 to n and of the sum of smallest prime factors of numbers
 *          from 1 to n.
 * https://oeis.org/A088823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( Σ_{k=1..n} lpf(k) , Σ_{k=1..n} spf(k) ) where lpf(k)
 *            is the largest prime factor of k and spf(k) is the smallest
 *            prime factor of k. This implementation computes the two
 *            cumulative sums once up to the maximum requested n and keeps
 *            them cached. The sums are stored in 64‑bit unsigned int...
 * Range:     u8 n <= 460, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *sum_lpf_u = NULL;   /* cumulative sum of largest prime factors */
static uint64_t *sum_spf_u = NULL;   /* cumulative sum of smallest prime factors */
static idx_t   *spf         = NULL;   /* smallest prime factor table */
static idx_t    cache_limit = 0;     /* largest n that has been cached */

/* Euclidean algorithm for 64‑bit integers. */
static unsigned long long gcd_u64(unsigned long long a, unsigned long long b)
{
    while (b != 0) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Ensure all arrays are computed up to at least n.  Rebuild from scratch
   whenever a larger n is requested. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_limit)
        return;   /* already cached */

    /* free any previous data */
    free((void *)sum_lpf_u);
    free((void *)sum_spf_u);
    free((void *)spf);
    sum_lpf_u = sum_spf_u = NULL;
    spf = NULL;

    /* allocate the smallest‑prime‑factor array */
    spf = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!spf)
        return;  /* allocation failed, leave cache empty */

    for (idx_t i = 0; i <= n; ++i)
        spf[i] = i;

    for (idx_t i = 2; i * i <= n; ++i)
        if (spf[i] == i)           /* i is prime */
            for (idx_t j = i * i; j <= n; j += i)
                if (spf[j] == j)
                    spf[j] = i;

    /* allocate cumulative‑sum arrays (use 64‑bit unsigned to avoid overflow) */
    sum_lpf_u = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    sum_spf_u = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!sum_lpf_u || !sum_spf_u) {
        free((void *)spf);
        spf = NULL;
        free((void *)sum_lpf_u);
        free((void *)sum_spf_u);
        sum_lpf_u = sum_spf_u = NULL;
        return;
    }

    sum_lpf_u[0] = sum_spf_u[0] = 0;
    sum_lpf_u[1] = sum_spf_u[1] = 0;   /* 1 has no prime factors */

    uint64_t cum_lpf = 0;
    uint64_t cum_spf = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* smallest prime factor contributes directly */
        cum_spf += (uint64_t)spf[i];
        sum_spf_u[i] = cum_spf;

        /* largest prime factor: factor i with spf table */
        idx_t tmp = i;
        idx_t lpf_val = 1;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            lpf_val = p;        /* last prime factor encountered */
            tmp /= p;
        }
        cum_lpf += (uint64_t)lpf_val;
        sum_lpf_u[i] = cum_lpf;
    }

    cache_limit = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* outside the offset, but harmless */

    ensure_cache(n);
    if (n > cache_limit)   /* memory allocation failed */
        return (seq_t)0;

    /* compute GCD of the two accumulated sums using 64‑bit arithmetic */
    unsigned long long g = gcd_u64(sum_lpf_u[n], sum_spf_u[n]);

    return (seq_t)g;   /* cast back to the required output type */
}
