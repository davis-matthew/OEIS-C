/*
 * A018761  Divisors of 988.
 * https://oeis.org/A018761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 988
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cmp_seq_t.h"

#define DIV_COUNT 12

seq_t a(idx_t n)
{
    static seq_t divs[DIV_COUNT];
    static bool init = false;

    if (!init) {
        init = true;
        size_t k = 0;
        for (int e2 = 0; e2 <= 2; ++e2) {
            for (int e13 = 0; e13 <= 1; ++e13) {
                for (int e19 = 0; e19 <= 1; ++e19) {
                    seq_t v = 1;
                    for (int i = 0; i < e2; ++i) v *= (seq_t)2;
                    for (int i = 0; i < e13; ++i) v *= (seq_t)13;
                    for (int i = 0; i < e19; ++i) v *= (seq_t)19;
                    divs[k++] = v;
                }
            }
        }
        qsort(divs, DIV_COUNT, sizeof(seq_t), cmp_seq_t);
    }

    if (n >= 1 && n <= DIV_COUNT) {
        return divs[(size_t)(n - 1)];
    }
    return (seq_t)0;
}
