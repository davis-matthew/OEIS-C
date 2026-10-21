/*
 * A072953  a(n)=sum(k=1,n,C(n,n reduced (mod k))).
 * https://oeis.org/A072953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} C(n, n mod k)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* for realloc */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* value for offset 0, not used by OEIS */

    /* Reusable array of binomial coefficients */
    static seq_t *nC = NULL;
    static size_t cap = 0;

    if ((size_t)(n + 1) > cap) {
        nC = (seq_t *)realloc(nC, (n + 1) * sizeof(seq_t));
        cap = n + 1;
    }

    /* Compute binomial coefficients C(n, r) for r = 0..n */
    nC[0] = 1;
    for (idx_t r = 1; r <= n; ++r) {
        seq_t v = nC[r - 1];
        v *= (seq_t)(n - r + 1);   /* exact integer multiplication */
        v /= (seq_t)r;             /* exact division */
        nC[r] = v;
    }

    /* Sum over k = 1..n of C(n, n mod k) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = n % k;           /* remainder n mod k */
        sum += nC[r];
    }

    return sum;
}
