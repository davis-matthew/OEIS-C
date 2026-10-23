/*
 * A036412  Number of empty intervals when fractional_part(i*e) for i = 1,
 *          ..., n is plotted along [ 0, 1 ] subdivided into n equal regions.
 * https://oeis.org/A036412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - |{ floor(i * n * e) mod n : 1 <= i <= n }|
 * Range:     u8 n <= 1291, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* High‑precision rational approximation of e:
     *  e = Σ_{k=0}^{20} 1/k!  + ε , |ε| < 1/21!  ≈ 2·10⁻¹⁹  */
    static __int128 e_num = 0;   /* numerator of the approximation  */
    static uint64_t e_den = 0;   /* denominator (20!)                 */
    static int init = 0;        /* once‑only initialisation          */

    if (!init) {
        uint64_t fact = 1;
        /* compute 20! */
        for (int k = 1; k <= 20; ++k) fact *= (uint64_t)k;
        e_den = fact;

        /* sum 20!/k! for k = 0 .. 20 */
        e_num = 0;
        uint64_t fac = 1;
        for (int k = 0; k <= 20; ++k) {
            e_num += (__int128)e_den / fac;   /* 20! / k!   */
            if (k < 20) fac *= (uint64_t)(k + 1);
        }
        init = 1;
    }

    if (n <= 1) return (seq_t)0;   /* a(1) = 0 */

    uint8_t *seen = (uint8_t *)calloc((size_t)n, sizeof(uint8_t));
    if (!seen) return (seq_t)0;     /* memory failure – unlikely */

    size_t distinct = 0;
    for (idx_t i = 1; i <= n; ++i) {
        __int128 prod = (__int128)i * (__int128)n * e_num;
        __int128 val  = prod / e_den;      /* floor(i*n*e)       */
        idx_t idx = (idx_t)(val % n);      /* interval index     */
        if (!seen[idx]) {
            seen[idx] = 1;
            ++distinct;
        }
    }
    free(seen);

    return (seq_t)(n - distinct);
}
