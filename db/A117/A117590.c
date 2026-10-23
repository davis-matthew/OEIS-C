/*
 * A117590  a(n) = ceiling(x(n)), where x(n) = 3*x(n-1)/2 and x(1) = 1.
 * https://oeis.org/A117590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(3^(n-1) / 2^(n-2)) (n ≥ 1, a(1)=2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *pow3 = NULL;           /* pow3[k] = 3^k , k ≥ 0   */
static size_t len3 = 0;              /* current size of pow3 array   */
static seq_t *pow2 = NULL;           /* pow2[k] = 2^k , k ≥ 0   */
static size_t len2 = 0;              /* current size of pow2 array   */

static void ensure_pow3(size_t k) {
    while (len3 <= k) {
        pow3 = (seq_t*)realloc(pow3, (len3 + 1) * sizeof(seq_t));
        if (!len3) {                 /* first element, 3^0 = 1      */
            pow3[0] = (seq_t)1;
        } else {
            pow3[len3] = pow3[len3 - 1] * (seq_t)3;
        }
        len3++;
    }
}

static void ensure_pow2(size_t k) {
    while (len2 <= k) {
        pow2 = (seq_t*)realloc(pow2, (len2 + 1) * sizeof(seq_t));
        if (!len2) {                 /* first element, 2^0 = 1      */
            pow2[0] = (seq_t)1;
        } else {
            pow2[len2] = pow2[len2 - 1] * (seq_t)2;
        }
        len2++;
    }
}

seq_t a(idx_t n) {
    if (n == 0) {                    /* undefined below offset, but return 0 */
        return (seq_t)0;
    }
    if (n == 1)                      /* a(1) = 2 */
        return (seq_t)2;

    /* For n ≥ 2:  a(n) = ceil(3^(n-1) / 2^(n-2)) */
    size_t exp3 = (size_t)(n - 1);   /* exponent for 3   */
    size_t exp2 = (size_t)(n - 2);   /* exponent for 2   */

    ensure_pow3(exp3);
    ensure_pow2(exp2);

    seq_t numerator   = pow3[exp3];
    seq_t denominator = pow2[exp2];

    /* ceil division: (numerator + denominator - 1) / denominator */
    return (numerator + denominator - (seq_t)1) / denominator;
}
