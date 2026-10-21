/*
 * A095538  a(n) = 73 written in base n.
 * https://oeis.org/A095538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal concatenation of the base‑n representation of 73
 *            * (for n = 1 the unary representation is used)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow10len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10len.h"

seq_t a(idx_t n)
{
    /* Unary (base 1) */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 73; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* base‑n representation of 73 */
    seq_t base = (seq_t)n;
    seq_t num  = 73;
    idx_t digits[16] = {0};
    idx_t dcnt = 0;

    /* collect digits least significant first */
    while (num > 0) {
        seq_t d = num % base;
        num = num / base;
        digits[dcnt++] = (idx_t)d;
    }

    /* build decimal concatenation from most significant digit */
    seq_t res = 0;
    for (idx_t i = 0; i < dcnt; ++i) {
        /* compute decimal length of this digit */
        int d = (int)digits[dcnt - 1 - i];
        int len = d < 10 ? 1
                          : (d < 100 ? 2
                                     : (d < 1000 ? 3 : 4));
        res = res * (seq_t)pow10len(len) + (seq_t)d;
    }

    return res;
}
