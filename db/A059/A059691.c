/*
 * A059691  Carryless product 12 X n base 10.
 * https://oeis.org/A059691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = carryless product of 12 and n in base 10 * * 12 in
 *            decimal is the digits [2,1] (least significant first). *
 *            Carryless multiplication is performed coefficient‑wise modulo
 *            10: * (a ⊗ b)[k] = Σ_{i+j=k} (a[i]·b[j]) mod 10 * * The routine
 *            below extracts the decimal digits of n (little‑endian),...
 * Range:     u8 n <= 21, u16 n <= 6299, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* idx_t and seq_t are defined by the build */

seq_t a(idx_t n)
{
    /* maximum decimal length of n (9 223 372 036 854 775 807 ≈ 1e19  has 19 digits) */
    unsigned char dig[25] = {0};      /* digits of n (least significant first) */
    unsigned char res[26] = {0};      /* carryless coefficient array */
    int len = 0;                      /* number of digits of n */

    /* decompose n into decimal digits */
    if (n == 0) {
        len = 1;
        dig[0] = 0;
    } else {
        idx_t tmp = n;
        while (tmp != 0) {
            dig[len++] = tmp % 10;
            tmp /= 10;
        }
    }

    /* carryless multiplication by the fixed digits 2 (0th place) and 1 (1st place) */
    for (int i = 0; i < len; ++i) {
        unsigned d = dig[i];                 /* digit of n */
        unsigned v2 = (2 * d) % 10;          /* 2·d mod 10, goes to position i      */
        unsigned v1 = d % 10;                /* 1·d mod 10, goes to position i+1    */
        res[i]     = (res[i] + v2) % 10;
        res[i + 1] = (res[i + 1] + v1) % 10;
    }

    /* determine the most significant non‑zero digit */
    int hi = len;                           /* potential extra slot at len */
    if (res[hi] == 0) hi = len - 1;         /* no carry into the extra slot    */

    /* convert the result digits back into a seq_t integer */
    seq_t value = 0;
    for (int i = hi; i >= 0; --i) {
        value = value * 10 + res[i];
    }

    return value;
}
