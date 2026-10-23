/*
 * A034719  Dirichlet convolution of powers of 3 (3,9,27,...) with themselves.
 * https://oeis.org/A034719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 3^(d + n/d)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Dynamically remember powers of 3 up to exponent n+1.
       pow_arr[e] holds 3^e.  pow_arr[0] is set to 1.          */
    static seq_t *pow_arr = NULL;
    static idx_t pow_size = 0;          /* amount of allocated entries */

    /* Allocate or grow the array so that pow_arr contains
       3^e for all e <= n+1.  We always keep at least index 2.   */
    if (pow_arr == NULL) {
        pow_size = n + 2;
        if (pow_size < 3) pow_size = 3;          /* ensure indices 0,1,2 exist */
        pow_arr = (seq_t *)malloc(sizeof(seq_t) * pow_size);
        pow_arr[0] = (seq_t)1;
        for (idx_t i = 1; i < pow_size; ++i)
            pow_arr[i] = pow_arr[i - 1] * (seq_t)3;
    } else if (n + 1 >= pow_size) {
        idx_t new_size = n + 2;
        if (new_size < pow_size) new_size = pow_size * 2;  /* safety growth */
        pow_arr = (seq_t *)realloc(pow_arr, sizeof(seq_t) * new_size);
        for (idx_t i = pow_size; i < new_size; ++i)
            pow_arr[i] = pow_arr[i - 1] * (seq_t)3;
        pow_size = new_size;
    }

    seq_t result = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;
            idx_t e = d + d2;               /* exponent for both divisors */
            result += pow_arr[e];
            if (d2 != d)
                result += pow_arr[e];       /* distinct divisor gives same exponent again */
        }
    }
    return result;
}
