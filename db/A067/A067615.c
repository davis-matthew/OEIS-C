/*
 * A067615  Terms in decimal expansion of 1/(17*2^n) before 5882352941176470
 *          (the period with no leading zeros of 1/17) appears.
 * https://oeis.org/A067615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = integer formed by the digits of 1/(17*2^n) preceding the
 *            first occurrence of 5882352941176470 in its decimal expansion
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"


seq_t a(idx_t n)
{
    seq_t denom = 17 * pow2(n);     /* 17 × 2^n */
    seq_t r = 1;

    size_t cap = 128;
    unsigned char *digits = (unsigned char*)malloc(cap);
    size_t len = 0;

    const unsigned char pat[16] = {5,8,8,2,3,5,2,9,4,1,1,7,6,4,7,0};

    while (1) {
        r = r * ((seq_t)10);
        seq_t dseq = r / denom;
        unsigned int d = (unsigned int)dseq;   /* 0 … 9 */
        r = r % denom;

        if (len == cap) {
            cap <<= 1;
            unsigned char *tmp = (unsigned char*)realloc(digits, cap);
            if (!tmp) { free(digits); return 0; }
            digits = tmp;
        }
        digits[len++] = (unsigned char)d;

        if (len >= 16) {
            bool ok = true;
            for (int i = 0; i < 16; ++i)
                if (digits[len-16+i] != pat[i]) { ok = false; break; }
            if (ok) break;
        }
    }

    size_t prefix_len = len - 16;
    seq_t res = 0;
    for (size_t i = 0; i < prefix_len; ++i)
        res = res * ((seq_t)10) + digits[i];

    free(digits);
    return res;
}
