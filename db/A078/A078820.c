/*
 * A078820  a(n) = 20*C(2n,n)*(2n+1)/(n+4).
 * https://oeis.org/A078820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*C(2n,n)*(2n+1)/(n+4) Recurrence used: a(0)=5,
 *            a(n)=a(n-1)*(4n^2+14n+6)/(n^2+4n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  include required headers for memory allocation and types  */
#include <stdlib.h>
#include <stdint.h>

/*  Function implementing the sequence  */
seq_t a(idx_t n)
{
    /* static cache for memoisation   */
    static seq_t *cache   = NULL;
    static idx_t  cache_size = 0;
    static idx_t  computed   = 0;   /* index of highest computed value */

    /*  allocate or grow the cache if necessary  */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;            /* out of memory          */
        cache[0] = (seq_t)5;                     /* a(0)=5                 */
        computed = 0;
    } else if (n + 1 > cache_size) {
        idx_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (tmp == NULL) return 0;               /* out of memory          */
        cache = tmp;
        cache_size = newsize;
    }

    /*  compute values up to n using the recurrence  */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t prev = cache[k - 1];

        /*  numerator and denominator of the recurrence  */
        idx_t num_fac = 4 * k * k + 14 * k + 6;   /* 4n^2+14n+6          */
        idx_t den_fac = k * k + 4 * k;             /*  n^2+4n             */

        seq_t val;
        /*  use 128‑bit intermediate when seq_t is a builtin integer  */
        if (sizeof(seq_t) <= 8) {
            __int128 prod = (__int128)prev * ( (__int128)num_fac );
            val = (seq_t)(prod / den_fac);
        } else {
            seq_t prod = prev * (seq_t)num_fac;
            val = prod / (seq_t)den_fac;
        }
        cache[k] = val;
    }
    computed = n;

    return cache[n];
}
