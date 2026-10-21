/*
 * A211588  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          four, six or eight distinct values for every i,j,k<=n.
 * https://oeis.org/A211588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 60*a(n-3) + 79*a(n-4) + 173*a(n-5) -
 *            334*a(n-6) - 186*a(n-7) + 504*a(n-8) + 76*a(n-9) - 296*a(n-10) -
 *            24*a(n-11) + 48*a(n-12) Initial values: a(1)=24, a(2)=76,
 *            a(3)=216, a(4)=576, a(5)=1500, a(6)=3904, a(7)=10088,
 *            a(8)=26400, a(9)=68820, a(10)=181908, a(11)=479096,
 *            a(12)=1277104
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 42, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t   cache_len = 0;          /* current number of terms stored */

    if (n == 0)    /* undefined, return 0 as placeholder */
        return (seq_t)0;

    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) /* allocation failure */
            abort();
        cache = new_cache;

        for (idx_t i = (idx_t)cache_len; i < n; ++i) {
            idx_t idx = i + 1;           /* term number (1‑based) */
            if (idx <= 12) {
                /* initial values, cast constants to seq_t for safety */
                switch (idx) {
                    case 1:  cache[i] = (seq_t)24;   break;
                    case 2:  cache[i] = (seq_t)76;   break;
                    case 3:  cache[i] = (seq_t)216;  break;
                    case 4:  cache[i] = (seq_t)576;  break;
                    case 5:  cache[i] = (seq_t)1500; break;
                    case 6:  cache[i] = (seq_t)3904; break;
                    case 7:  cache[i] = (seq_t)10088;break;
                    case 8:  cache[i] = (seq_t)26400;break;
                    case 9:  cache[i] = (seq_t)68820;break;
                    case 10: cache[i] = (seq_t)181908;break;
                    case 11: cache[i] = (seq_t)479096;break;
                    case 12: cache[i] = (seq_t)1277104;break;
                }
            } else {
                /* indices in cache are 0‑based, so cache[i‑k] is a(idx‑k) */
                seq_t t1  = cache[i-1];
                seq_t t3  = cache[i-3];
                seq_t t4  = cache[i-4];
                seq_t t5  = cache[i-5];
                seq_t t6  = cache[i-6];
                seq_t t7  = cache[i-7];
                seq_t t8  = cache[i-8];
                seq_t t9  = cache[i-9];
                seq_t t10 = cache[i-10];
                seq_t t11 = cache[i-11];
                seq_t t12 = cache[i-12];

                /* group positive and negative contributions to avoid
                   unsigned underflow when subtracting */
                seq_t pos = (seq_t)6  * t1
                          + (seq_t)79 * t4
                          + (seq_t)173 * t5
                          + (seq_t)504 * t8
                          + (seq_t)76  * t9
                          + (seq_t)48  * t12;

                seq_t neg = (seq_t)60  * t3
                          + (seq_t)334 * t6
                          + (seq_t)186 * t7
                          + (seq_t)296 * t10
                          + (seq_t)24  * t11;

                cache[i] = pos - neg;
            }
        }
        cache_len = new_len;
    }

    return cache[n-1];
}
