/*
 * A166406  a(n) = A166405(n)-A166100(n).
 * https://oeis.org/A166406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=1}^{2n+1, J(i,2n+1)=-1} i - Sum_{i=1}^{2n+1,
 *            J(i,2n+1)=+1} i where J(i,k) is the Jacobi symbol (i|k)
 * Range:     i8 n <= 11, i16 n <= 143, i32 n <= 9581, i64 n <= 9581,
 *            i128 n <= 9581, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int jacobi_symbol(uint64_t a, uint64_t n)
{
    /* Compute the Jacobi symbol (a|n) for odd n > 0. */
    if (n == 0) return 0;
    int result = 1;
    a %= n;
    while (a != 0) {
        while ((a & 1) == 0) {                /* a even */
            a >>= 1;
            uint64_t r = n & 7ULL;            /* n mod 8 */
            if (r == 3 || r == 5)
                result = -result;
        }
        /* Swap a and n */
        uint64_t temp = a; a = n; n = temp;
        if ((a & 3) == 3 && (n & 3) == 3)
            result = -result;
        a %= n;
    }
    return (n == 1) ? result : 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)(-1);          /* as specified for the offset 0 */

    idx_t k = (n << 1) + 1;          /* k = 2n + 1 (odd) */
    seq_t sum = 0;

    for (idx_t i = 1; i < k; ++i) {
        int js = jacobi_symbol((uint64_t)i, (uint64_t)k);    /* Jacobi symbol */
        if (js != 0) {
            sum += (seq_t)i * (seq_t)(-js);   /* add if js=-1, subtract if js=+1 */
        }
    }
    return sum;
}
