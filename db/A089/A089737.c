/*
 * A089737  Number of (1,1) steps starting at level zero in all peakless
 *          Motzkin paths of length n+3.
 * https://oeis.org/A089737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=ceiling(n/2+1)..n+1}
 *            (5k-2n-2)*C(k,n+1-k)*C(k+1,n+3-k)/(k*(n+4-k))
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 13, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (used for calculation): -(n+7)(86n-51)a(n)+3(95n^2+434n-249)a(n-1)+4(-35n^2-71n-60)a(n-2)+(59n^2+209n+1020)a(n-3)+6(-52n^2+11n+71)a(n-4)+(113n+119)(n-3)a(n-5)=0 */
/* Initial values: a(0)=1, a(1)=3, a(2)=7, a(3)=17, a(4)=41 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* ensure cache holds entries up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;
    size_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return; /* silently abort if allocation fails; not expected */
    cache = new_cache;

    /* fill missing values with recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) cache[0] = 1;
        else if (i == 1) cache[1] = 3;
        else if (i == 2) cache[2] = 7;
        else if (i == 3) cache[3] = 17;
        else if (i == 4) cache[4] = 41;
        else {
            /* compute a(i) via recurrence using previously computed terms */
            int64_t N = (int64_t)i;

            /* coefficients (using signed 64-bit to avoid overflow) */
            int64_t coeff1 = 3LL * (95LL * N * N + 434LL * N - 249LL);          /* positive */
            int64_t coeff3 = 59LL * N * N + 209LL * N + 1020LL;                /* positive */
            int64_t coeff5 = (113LL * N + 119LL) * (N - 3LL);                 /* positive */

            int64_t coeff2 = 4LL * (-35LL * N * N - 71LL * N - 60LL);          /* negative */
            int64_t coeff4 = 6LL * (-52LL * N * N + 11LL * N + 71LL);          /* negative */

            /* positive parts */
            seq_t pos = (seq_t)coeff1 * cache[i - 1] +
                        (seq_t)coeff3 * cache[i - 3] +
                        (seq_t)coeff5 * cache[i - 5];

            /* negative parts (use absolute values) */
            seq_t neg = (seq_t)(-coeff2) * cache[i - 2] +
                        (seq_t)(-coeff4) * cache[i - 4];

            /* numerator = pos - neg (positive by construction) */
            seq_t numerator = pos - neg;

            /* denominator */
            seq_t denom = (seq_t)((i + 7) * (86LL * i - 51LL));

            cache[i] = numerator / denom;
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* direct return for first five terms */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 7;
    if (n == 3) return 17;
    if (n == 4) return 41;

    ensure_cache(n);
    return cache[n];
}
