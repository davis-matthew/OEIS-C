/*
 * A055035  Degree of minimal polynomial of sin(Pi/n) over the rationals.
 * https://oeis.org/A055035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=1; for n>2, a(n)=phi(n)/(1 if n%4!=2 else 2)
 * Range:     u8 n <= 175, u16 n <= 175, u32 n <= 175, u64 n <= 175,
 *            u128 n <= 175, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 2) return 1;

    idx_t temp = n;
    seq_t phi = n;  /* phi(n) initialized to n */

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            phi -= phi / p;
        }
    }
    if (temp > 1) phi -= phi / temp;

    /* divisor is 2 when n mod 4 == 2, else 1 */
    seq_t divisor = (n & 3) == 2 ? 2 : 1;
    return phi / divisor;
}
