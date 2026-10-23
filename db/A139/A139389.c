/*
 * A139389  Powers of ten in factorial base.
 * https://oeis.org/A139389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A007623(A011557(n)) – the factoradic representation of
 *            10^n written as a decimal number
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n. */
    seq_t val = pow10(n);

    /* Dynamically grow an array of factoradic digits. */
    size_t cap = 16;
    uint32_t *digits = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t cnt = 0;
    idx_t i = 1;                     /* current base minus 1 */

    while (val != 0) {
        seq_t divisor = (seq_t)(i + 1);
        seq_t rem = val % divisor;
        if (cnt == cap) {            /* grow the buffer */
            cap <<= 1;
            uint32_t *tmp = (uint32_t *)realloc(digits, cap * sizeof(uint32_t));
            if (!tmp) {              /* allocation failure – give up */
                free(digits);
                return 0;
            }
            digits = tmp;
        }
        digits[cnt++] = (uint32_t)rem;
        val = val / divisor;
        ++i;
    }

    /* Concatenate the digits into a decimal number. */
    seq_t result = 0;
    for (long long j = (long long)cnt - 1; j >= 0; --j) {
        uint32_t d = digits[j];

        /* Compute the number of decimal digits of d. */
        uint32_t tmp = d;
        uint32_t len = 1;
        while (tmp >= 10) {
            tmp /= 10;
            ++len;
        }

        /* Compute 10^len and append d. */
        seq_t pow10len = 1;
        for (uint32_t k = 0; k < len; ++k) {
            pow10len *= (seq_t)10;
        }
        result = result * pow10len + (seq_t)d;
    }

    free(digits);
    return result;
}
