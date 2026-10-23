/*
 * A177847  Array T(n,m)= (n*m)!*Beta(n, m) read by antidiagonals.
 * https://oeis.org/A177847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = Gamma(n*m+1)*Gamma(n)*Gamma(m)/Gamma(n+m) * any term
 *            a(k) is obtained by reading T(n,m) along anti‑diagonals, * in
 *            the order used by OEIS for A177847
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 23, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t  (idx_t)               */

/*----- integer square root for uint64_t (floor) --------------------------------*/
static unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long lo = 0, hi = 1ull << 32; /* > sqrt(2^64-1) */
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/*----- small factorial cache ---------------------------------------------------*/
#define FACT_MAX 200
static seq_t fact_cache[FACT_MAX + 1];
static int fact_cache_ready = 0;

static void init_fact_cache(void)
{
    if (fact_cache_ready)
        return;
    fact_cache[0] = (seq_t)1;
    for (unsigned int i = 1; i <= FACT_MAX; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    fact_cache_ready = 1;
}

static seq_t small_fact(idx_t n)
{
    if (n <= FACT_MAX)
        return fact_cache[(unsigned int)n];
    seq_t res = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
        res *= (seq_t)k;
    return res;
}

/*----- main entry -------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined but harmless */
        return (seq_t)0;

    init_fact_cache();

    /* find diagonal sum s so that (s-1)*s/2 >= n   (s >= 2) */
    unsigned long long t = isqrt_u64(1 + 8ull * n);
    unsigned long long s = (1 + t) >> 1;          /* floor((1+sqrt)/2) */
    if (((unsigned __int128)s * (s - 1)) / 2 < n)
        ++s;

    /* offset within the diagonal (0‑based) */
    unsigned long long prev_terms = ((s - 2) * (s - 1)) / 2;
    unsigned long long offset   = n - prev_terms - 1;          /* 0‑based */

    idx_t i = (idx_t)(offset + 1);         /* row index, 1‑based */
    idx_t j = (idx_t)(s - i);              /* column index, 1‑based */

    /* compute T(i,j) = (i-1)! * (j-1)! * product_{k=i+j}^{i*j} k   */
    seq_t result = small_fact(i - 1);
    result *= small_fact(j - 1);

    if (i + j <= i * j) {
        for (idx_t k = i + j; k <= i * j; ++k)
            result *= (seq_t)k;
    }

    return result;
}
