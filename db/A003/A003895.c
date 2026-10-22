/*
 * A003895  Degrees of irreducible representations of group L3(4).
 * https://oeis.org/A003895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   finite sequence (10 terms). Degrees of the irreducible
 *            characters of PSL(3,q) for q = 4, taken from the generic
 *            GL(3,q)/SL(3,q) character-degree polynomials: 1, q(q+1), q^3,
 *            (q-1)^2(q+1) (x2), (q-1)(q^2+q+1) (x2) and (q+1)(q^2+q+1)/3 (x3,
 *            which split because gcd(3,q-1)=3). The degrees are sorted...
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t q = (seq_t)4;
    seq_t d[10];
    int k = 0, i, j;
    d[k++] = (seq_t)1;
    d[k++] = q * (q + 1);
    for (i = 0; i < 3; i++)
        d[k++] = (q + 1) * (q * q + q + 1) / 3;
    for (i = 0; i < 2; i++)
        d[k++] = (q - 1) * (q - 1) * (q + 1);
    for (i = 0; i < 2; i++)
        d[k++] = (q - 1) * (q * q + q + 1);
    d[k++] = q * q * q;
    /* insertion sort */
    for (i = 1; i < k; i++) {
        seq_t t = d[i];
        j = i - 1;
        while (j >= 0 && d[j] > t) {
            d[j + 1] = d[j];
            j--;
        }
        d[j + 1] = t;
    }
    if (n < 1 || n > (idx_t)k)
        return (seq_t)0;
    return d[n - 1];
}
