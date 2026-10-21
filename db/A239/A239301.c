/*
 * A239301  E.g.f.: exp((1-5*x)^(-1/5)-1)/(1-5*x).
 * https://oeis.org/A239301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=67, a(3)=1090, a(4)=23265, a(5)=614302 For
 *            n>=6: a(n)=5*(6n-13)*a(n-1) -5*(75n²-400n+557)*a(n-2)
 *            +50*(50n³-475n²+1539n-1698)*a(n-3)
 *            -(9375n⁴-137500n³+764625n²-1910000n+1807524)*a(n-4)
 *            +(18750n⁵-390625n⁴+3267500n³-13716875n²+28896490n-24436079)*a(n-5)
 *            -25*(n-5)²*(5n-24)*(5n-23...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t base[6] = {
        (seq_t)1,
        (seq_t)6,
        (seq_t)67,
        (seq_t)1090,
        (seq_t)23265,
        (seq_t)614302
    };

    if (n < cache_size) {
        return cache[n];
    }

    /* grow cache to hold values up to n */
    size_t new_sz = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t*)malloc(new_sz * sizeof(seq_t));
    } else {
        cache = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
    }
    if (cache == NULL) {               /* allocation failure, return 0 as a fallback */
        return (seq_t)0;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 6) {
            cache[i] = base[i];
        } else {
            seq_t ni  = (seq_t)i;
            seq_t n2  = ni * ni;
            seq_t n3  = n2 * ni;
            seq_t n4  = n3 * ni;
            seq_t n5  = n4 * ni;

            /* coefficients */
            seq_t coeff1 = (seq_t)5 * ((seq_t)6 * ni - (seq_t)13);
            seq_t coeff2 = (seq_t)5 * ((seq_t)75 * n2 - (seq_t)400 * ni + (seq_t)557);
            seq_t coeff3 = (seq_t)50 * ((seq_t)50 * n3 - (seq_t)475 * n2 + (seq_t)1539 * ni - (seq_t)1698);
            seq_t coeff4 = (seq_t)9375 * n4 - (seq_t)137500 * n3 + (seq_t)764625 * n2 -
                           (seq_t)1910000 * ni + (seq_t)1807524;
            seq_t coeff5 = (seq_t)18750 * n5 - (seq_t)390625 * n4 + (seq_t)3267500 * n3 -
                           (seq_t)13716875 * n2 + (seq_t)28896490 * ni - (seq_t)24436079;
            seq_t d      = (seq_t)(i - 5);
            seq_t coeff6 = (seq_t)25 * d * d *
                           ((seq_t)5 * ni - (seq_t)24) *
                           ((seq_t)5 * ni - (seq_t)23) *
                           ((seq_t)5 * ni - (seq_t)22) *
                           ((seq_t)5 * ni - (seq_t)21);

            /* recurrence computation */
            seq_t term1 = coeff1 * cache[i - 1];
            seq_t term2 = coeff2 * cache[i - 2];
            seq_t term3 = coeff3 * cache[i - 3];
            seq_t term4 = coeff4 * cache[i - 4];
            seq_t term5 = coeff5 * cache[i - 5];
            seq_t term6 = coeff6 * cache[i - 6];

            cache[i] = term1 - term2 + term3 - term4 + term5 - term6;
        }
    }

    cache_size = new_sz;
    return cache[n];
}
