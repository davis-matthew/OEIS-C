/*
 * A176956  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=3, k=-1 and l=-1.
 * https://oeis.org/A176956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176956, defined by the recurrence
   a(0)=1, a(1)=3, a(2)=3, a(3)=11, a(4)=35, a(5)=139,
   and for n≥6
     (n+1)*a(n) +(-7*n+2)*a(n-1) +(7*n-5)*a(n-2) +(23*n-82)*a(n-3)
    +4*(-10*n+41)*a(n-4) +16*(n-5)*a(n-5) = 0.
   The function uses a static cache to allow arbitrary calls in any order. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* cache of already computed terms          */
static size_t cache_len = 0;     /* number of allocated elements             */
static idx_t  last_comp = 0;     /* greatest n that has been computed so far */

/* Helper: initialise first six terms if necessary */
static void init_base(void)
{
    if (cache_len > 0) cache[0] = (seq_t)1;
    if (cache_len > 1) cache[1] = (seq_t)3;
    if (cache_len > 2) cache[2] = (seq_t)3;
    if (cache_len > 3) cache[3] = (seq_t)11;
    if (cache_len > 4) cache[4] = (seq_t)35;
    if (cache_len > 5) cache[5] = (seq_t)139;
}

seq_t a(idx_t n)
{
    /* grow cache if needed */
    if ((size_t)(n + 1) > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failed */

        /* copy previously stored terms */
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;

        /* initialise initial values */
        init_base();

        /* update last computed index */
        last_comp = (cache_len > 5 ? 5 : cache_len - 1);
    }

    /* compute missing terms up to n */
    if (n > (idx_t)last_comp) {
        size_t start = (size_t)last_comp + 1;
        size_t stop  = (size_t)n;

        for (size_t i = start; i <= stop; ++i) {
            /* coefficients are all linear in i (0‑based index) */
            int64_t c1 = -7 * (int64_t)i + 2;          /* a[i‑1] coeff */
            int64_t c2 =  7 * (int64_t)i - 5;          /* a[i‑2] coeff */
            int64_t c3 = 23 * (int64_t)i - 82;         /* a[i‑3] coeff */
            int64_t c4 = 4 * ((-10 * (int64_t)i + 41));/* a[i‑4] coeff */
            int64_t c5 = 16 * ((int64_t)i - 5);        /* a[i‑5] coeff */

            seq_t sum = (seq_t)0;
            sum += (seq_t)c1 * cache[i-1];
            sum += (seq_t)c2 * cache[i-2];
            sum += (seq_t)c3 * cache[i-3];
            sum += (seq_t)c4 * cache[i-4];
            sum += (seq_t)c5 * cache[i-5];

            seq_t denom = (seq_t)(i + 1);
            cache[i] = -sum / denom;
        }
        last_comp = n;
    }

    return cache[n];
}
