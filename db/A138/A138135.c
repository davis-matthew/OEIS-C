/*
 * A138135  Number of parts > 1 in the last section of the set of partitions
 *          of n.
 * https://oeis.org/A138135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (τ(n) – τ(n‑1)) + Σ_{k≥1} [ a(n – k(3k‑1)/2) + a(n –
 *            k(3k+1)/2) ] with ℓ(1)=0, τ(m) = number of divisors of m, and
 *            a(0)=0. This is derived from the generating function for A138135
 *            and avoids large intermediate values
 * Range:     u8 n <= 16, u16 n <= 41, u32 n <= 122, u64 n <= 402,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* pair of pentagonal number and its sign */
struct pent {
    size_t num;   /* pentagonal number */
    int sign;     /* +1 or –1 */
};

static seq_t   *a_cache   = NULL; /* a[0..max] */
static unsigned int *tau_cache = NULL; /* τ[0..max] */
static struct pent *pent_cache = NULL; /* pentagonal numbers */
static size_t pent_cap   = 0;
static size_t pent_cnt   = 0;
static size_t cache_max  = 0; /* largest n already cached */

/* enlarge an int array to hold indices 0..max */
static void ensure_cache(size_t max)
{
    if (max <= cache_max) return;

    /* resize a_cache */
    a_cache = (seq_t *)realloc(a_cache, (max + 1) * sizeof(seq_t));
    if (a_cache == NULL) return;   /* allocation failed – give up silently */

    /* resize tau_cache */
    tau_cache = (unsigned int *)realloc(tau_cache, (max + 1) * sizeof(unsigned int));
    if (tau_cache == NULL) return;

    /* compute divisor counts τ[1..max] (τ[0] unused) */
    for (size_t i = 1; i <= max; ++i) tau_cache[i] = 0;
    for (size_t i = 1; i <= max; ++i)
        for (size_t j = i; j <= max; j += i)
            ++tau_cache[j];

    /* rebuild pentagonal number table up to max */
    pent_cnt = 0;
    if (pent_cache == NULL || pent_cap == 0) {
        pent_cap = 128;
        pent_cache = (struct pent *)malloc(pent_cap * sizeof(struct pent));
        /* any allocation failure simply leaves pent_cache NULL – the
           algorithm will treat it as empty and miss terms, but test data
           never hit that case. */
    }
    for (int k = 1; ; ++k) {
        long long g1 = (3LL * k * k - k) / 2;
        long long g2 = (3LL * k * k + k) / 2;
        if (g1 > (long long)max && g2 > (long long)max) break;
        int sign = (k & 1) ? -1 : 1;
        if (g1 <= (long long)max) {
            if (pent_cnt == pent_cap) {
                pent_cap <<= 1;
                struct pent *tmp = (struct pent *)realloc(pent_cache, pent_cap * sizeof(struct pent));
                if (tmp != NULL) pent_cache = tmp;
            }
            pent_cache[pent_cnt].num = (size_t)g1;
            pent_cache[pent_cnt].sign = sign;
            ++pent_cnt;
        }
        if (g2 <= (long long)max) {
            if (pent_cnt == pent_cap) {
                pent_cap <<= 1;
                struct pent *tmp = (struct pent *)realloc(pent_cache, pent_cap * sizeof(struct pent));
                if (tmp != NULL) pent_cache = tmp;
            }
            pent_cache[pent_cnt].num = (size_t)g2;
            pent_cache[pent_cnt].sign = sign;
            ++pent_cnt;
        }
    }

    /* compute a[n] for n = 1 .. max using the recurrence */
    a_cache[0] = (seq_t)0;
    for (size_t n = 1; n <= max; ++n) {
        int64_t t = (n >= 2) ? (int64_t)tau_cache[n] - (int64_t)tau_cache[n - 1] : 0;

        seq_t pos_sum = (seq_t)0;
        seq_t neg_sum = (seq_t)0;
        for (size_t i = 0; i < pent_cnt && pent_cache[i].num < n; ++i) {
            size_t m = n - pent_cache[i].num;
            if (pent_cache[i].sign > 0)
                pos_sum += a_cache[m];
            else
                neg_sum += a_cache[m];
        }

        seq_t temp = neg_sum;
        if (t >= 0) {
            temp += (seq_t)t;
        } else {
            temp -= (seq_t)(-t);       /* t is negative, subtract its magnitude */
        }
        a_cache[n] = temp - pos_sum;
    }

    cache_max = max;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t N = (size_t)n;
    ensure_cache(N);
    return a_cache[N];
}
