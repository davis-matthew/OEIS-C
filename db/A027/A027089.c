/*
 * A027089  a(n) = A027082(n, 2n-2).
 * https://oeis.org/A027089
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A027082(n, 2n-2) Recurrence (order 7):
 *            -(n+2)*(244*n-1065)*a(n) +3*(n+1)*(422*n-1703)*a(n-1)
 *            +2*(-359*n^2+1211*n-1420)*a(n-2)
 *            +2*(-1174*n^2+6319*n-7410)*a(n-3) +(-760*n^2+2173*n+1616)*a(n-4)
 *            +(-502*n^2+2245*n-1123)*a(n-5) +6*(23*n-67)*(n-5)*a(n-6) = 0
 *            Initial values: a(2)=1, a(3)=5, a(4)=1...
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 12, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#endif
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 2.  For smaller n we return 0. */
    if (n < 2) {
        return (seq_t)0;
    }

#if defined(__cplusplus)
    /* C++ variant: use std::vector for automatic construction/destruction. */
    static std::vector<seq_t> cache;
    static idx_t computed = 1;          /* highest index computed so far */

    /* Ensure the vector can hold index n. */
    if (cache.size() <= n) {
        cache.resize(n + 1);
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        switch (i) {
            case 2: cache[i] = (seq_t)1; break;
            case 3: cache[i] = (seq_t)5; break;
            case 4: cache[i] = (seq_t)14; break;
            case 5: cache[i] = (seq_t)39; break;
            case 6: cache[i] = (seq_t)108; break;
            case 7: cache[i] = (seq_t)299; break;
            default: {
                /* Use seq_t arithmetic for all intermediate values. */
                seq_t tn   = (seq_t)i;

                /* Coefficients as seq_t.  Negative coefficients are allowed
                   because the arbitrary‑precision type handles signed values. */
                seq_t coeff1 = (seq_t)3 * (tn + (seq_t)1)
                               * ((seq_t)422 * tn - (seq_t)1703);
                seq_t coeff2 = (seq_t)2 * ((- (seq_t)359 * tn * tn
                                            + (seq_t)1211 * tn
                                            - (seq_t)1420));
                seq_t coeff3 = (seq_t)2 * ((- (seq_t)1174 * tn * tn
                                            + (seq_t)6319 * tn
                                            - (seq_t)7410));
                seq_t coeff4 = - (seq_t)760 * tn * tn
                               + (seq_t)2173 * tn
                               + (seq_t)1616;
                seq_t coeff5 = - (seq_t)502 * tn * tn
                               + (seq_t)2245 * tn
                               - (seq_t)1123;
                seq_t coeff6 = (seq_t)6 * ((seq_t)23 * tn - (seq_t)67)
                               * (tn - (seq_t)5);

                /* Numerator: sum of products. */
                seq_t numerator = coeff1 * cache[i-1]
                                + coeff2 * cache[i-2]
                                + coeff3 * cache[i-3]
                                + coeff4 * cache[i-4]
                                + coeff5 * cache[i-5]
                                + coeff6 * cache[i-6];

                /* Denominator. */
                seq_t denominator = (tn + (seq_t)2)
                                    * ((seq_t)244 * tn - (seq_t)1065);

                /* Division yields a(n).  The recurrence guarantees an exact
                   integer result. */
                cache[i] = numerator / denominator;
            }
        }
    }
    computed = n;
    return cache[n];
#else
    /* C variant: use malloc/realloc for a raw array of seq_t. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;     /* allocated size of cache */
    static idx_t computed = 1;      /* highest index whose value is known */

    /* Ensure the array can hold index n. */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap <= n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort();
        }
        /* Initialize newly allocated slots to zero (value 0). */
        for (idx_t i = cache_cap; i < new_cap; ++i) {
            tmp[i] = (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        switch (i) {
            case 2: cache[i] = (seq_t)1; break;
            case 3: cache[i] = (seq_t)5; break;
            case 4: cache[i] = (seq_t)14; break;
            case 5: cache[i] = (seq_t)39; break;
            case 6: cache[i] = (seq_t)108; break;
            case 7: cache[i] = (seq_t)299; break;
            default: {
                /* Use seq_t arithmetic for all intermediate values. */
                seq_t tn   = (seq_t)i;

                /* Coefficients as seq_t, allowing negative values. */
                seq_t coeff1 = (seq_t)3 * (tn + (seq_t)1)
                               * ((seq_t)422 * tn - (seq_t)1703);
                seq_t coeff2 = (seq_t)2 * ((- (seq_t)359 * tn * tn
                                            + (seq_t)1211 * tn
                                            - (seq_t)1420));
                seq_t coeff3 = (seq_t)2 * ((- (seq_t)1174 * tn * tn
                                            + (seq_t)6319 * tn
                                            - (seq_t)7410));
                seq_t coeff4 = - (seq_t)760 * tn * tn
                               + (seq_t)2173 * tn
                               + (seq_t)1616;
                seq_t coeff5 = - (seq_t)502 * tn * tn
                               + (seq_t)2245 * tn
                               - (seq_t)1123;
                seq_t coeff6 = (seq_t)6 * ((seq_t)23 * tn - (seq_t)67)
                               * (tn - (seq_t)5);

                seq_t numerator = coeff1 * cache[i-1]
                                + coeff2 * cache[i-2]
                                + coeff3 * cache[i-3]
                                + coeff4 * cache[i-4]
                                + coeff5 * cache[i-5]
                                + coeff6 * cache[i-6];

                seq_t denominator = (tn + (seq_t)2)
                                    * ((seq_t)244 * tn - (seq_t)1065);

                cache[i] = numerator / denominator;
            }
        }
    }
    computed = n;
    return cache[n];
#endif
}
