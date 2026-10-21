/*
 * A098481  Expansion of 1/sqrt((1-x)^2 - 12*x^3).
 * https://oeis.org/A098481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{k=0..⌊n/2⌋} binomial(n-k,k) * binomial(n-2k,k) * 3^k
 * Recurrence: n*a(n) = (2*n-1)*a(n-1) - (n-1)*a(n-2) + 6*(2*n-3)*a(n-3)
 * Initial values: a(0)=a(1)=a(2)=1
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t count = 0;

    /* initialize cache with the first three values */
    if (cache == NULL) {
        count = 3;
        cap   = 4;                       /* a little extra space */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;     /* allocation failed */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
    }

    /* return value if already computed */
    if (n < count) return cache[n];

    /* compute values up to n */
    for (idx_t i = count; i <= n; ++i) {
        /* enlarge cache if necessary */
        if (i >= cap) {
            size_t new_cap = (cap > 0) ? cap * 2 : 4;
            while (new_cap <= (size_t)i) new_cap <<= 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (cache == NULL) return 0;
            cap = new_cap;
        }

        seq_t val;
#ifdef __cplusplus
        /* C++/big‑int path – use seq_t arithmetic directly */
        seq_t si = (seq_t)i;                  /* cast index to seq_t */
        seq_t t1 = ( ( (si << 1) - 1 ) * cache[i-1] );          /* (2*i-1)*a[i-1] */
        seq_t t2 = ( (si - 1) * cache[i-2] );                   /* (i-1)*a[i-2]   */
        seq_t t3 = ( 6 * ( (si << 1) - 3 ) * cache[i-3] );      /* 6*(2*i-3)*a[i-3] */
        seq_t num = t1 - t2 + t3;
        val = num / si;
#else
        /* C path – use __int128 for intermediate overflow safety */
        __int128 si = ( __int128)i;
        __int128 t1 = ( ( (si << 1) - 1 ) * cache[i-1] );          /* (2*i-1)*a[i-1] */
        __int128 t2 = ( (si - 1) * cache[i-2] );                   /* (i-1)*a[i-2]   */
        __int128 t3 = ( 6 * ( (si << 1) - 3 ) * cache[i-3] );      /* 6*(2*i-3)*a[i-3] */
        __int128 num = t1 - t2 + t3;
        val = (seq_t)( num / si );
#endif
        cache[i] = val;
    }

    count = n + 1;
    return cache[n];
}
