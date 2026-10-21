/*
 * A095561  a(n) = 84 written in base 16 - n.
 * https://oeis.org/A095561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = decimal integer obtained by writing 84 in base (16-n).
   For example,
      n=0  → base 16:  84₁₆ = 54₁₀ → a(0)=54
      n=1  → base 15:  84₁₅ = 59₁₀ → a(1)=59
      ...
      n=14 → base  2:  84₂  =1010100₂ → a(14)=1010100
      n=15 → base  1:  84₁  = 84 ones   → a(15)=111…111   (84 digits)

   For base = 1, the value consists of 84 ones.
   For base 0 or –1 no standard representation exists; a(n)=0 in those cases.
   For negative bases b ≤ –2, a representation exists with digits 0,…,|b|−1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute base b = 16 - n */
    long long base = 16 - (long long)n;

    /* base 1 → unary representation of 84 ones */
    if (base == 1) {
        seq_t res = 0;
        for (int i = 0; i < 84; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* base 0 or -1 : undefined, return 0 */
    if (base == 0 || base == -1)
        return 0;

    /* work with absolute value of base for digit extraction */
    int absBase;
    int isNegBase = 0;
    if (base > 1) {
        absBase = (int)base;
    } else { /* base < -1 */
        absBase = (int)(-base);
        isNegBase = 1;
    }

    /* number to convert is always 84 */
    int val = 84;
    int digits[64];
    int dcnt = 0;

    if (!isNegBase) { /* positive base */
        while (val > 0) {
            digits[dcnt++] = val % absBase;
            val /= absBase;
        }
    } else { /* negative base */
        while (val != 0) {
            int r = val % absBase;
            if (r < 0) r += absBase;
            digits[dcnt++] = r;
            val = (val - r) / base; /* base is negative */
        }
    }

    /* assemble the decimal integer from the digits (most significant first) */
    seq_t res = 0;
    for (int i = dcnt - 1; i >= 0; --i)
        res = res * 10 + digits[i];

    return res;
}
