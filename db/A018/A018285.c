/*
 * A018285  Divisors of 104.
 * https://oeis.org/A018285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 104 in ascending order (0 if n > 8)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache the divisors of 104 */
    static seq_t *divs   = NULL;
    static size_t div_cnt = 0;
    static size_t div_cap = 0;

    if (!divs) {
        /* 104 = 2^3 * 13^1 -> (3+1)*(1+1) = 8 divisors */
        div_cnt = 8;
        div_cap = 8;
        divs = (seq_t *)malloc(div_cnt * sizeof(seq_t));
        if (!divs) return 0;          /* allocation failed */

        size_t pos = 0;
        for (unsigned i = 0; i <= 3; ++i) {
            seq_t p2 = 1;
            for (unsigned k = 0; k < i; ++k) p2 *= 2;   /* 2^i */
            for (unsigned j = 0; j <= 1; ++j) {
                seq_t prod = p2;
                for (unsigned k = 0; k < j; ++k) prod *= 13;   /* *13^j */
                divs[pos++] = prod;
            }
        }

        /* sort ascending */
        for (size_t i = 0; i + 1 < div_cnt; ++i)
            for (size_t j = i + 1; j < div_cnt; ++j)
                if (divs[i] > divs[j]) {
                    seq_t t = divs[i];
                    divs[i] = divs[j];
                    divs[j] = t;
                }
    }

    if ((size_t)n >= 1 && (size_t)n <= div_cnt)
        return divs[(size_t)n - 1];
    return (seq_t)0;
}
