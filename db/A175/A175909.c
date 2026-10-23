/*
 * A175909  Decimal representation of the binary number formed by the
 *          concatenation of digits which are the same in both the left and
 *          right binary concatenations of the integers 1 to n.
 * https://oeis.org/A175909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal value obtained from the binary string * formed by
 *            the bits that are identical in the concatenation * of the binary
 *            representations of 1..n and of n..1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* return the number of bits needed to represent x (x>0) */
static inline int bitlen_uint64(uint64_t x)
{
    int len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, safe return */

    /* total length of both concatenations */
    uint64_t len = 0;
    for (idx_t i = 1; i <= n; ++i)
        len += bitlen_uint64((uint64_t)i);

    /* allocate bit arrays */
    uint8_t *f = (uint8_t*)malloc(len);   /* forward concatenation */
    uint8_t *r = (uint8_t*)malloc(len);   /* reverse concatenation   */
    if (!f || !r) return (seq_t)0;       /* allocation failure */

    /* build forward bits: 1,2,...,n */
    uint64_t pos = 0;
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t val = (uint64_t)i;
        int bits = bitlen_uint64(val);
        for (int k = bits - 1; k >= 0; --k)
            f[pos++] = (uint8_t)((val >> k) & 1U);
    }

    /* build reverse bits: n,...,1 */
    pos = 0;
    for (idx_t ii = n; ii > 0; --ii) {
        uint64_t val = (uint64_t)ii;
        int bits = bitlen_uint64(val);
        for (int k = bits - 1; k >= 0; --k)
            r[pos++] = (uint8_t)((val >> k) & 1U);
    }

    /* produce result from matching bits */
    seq_t res = 0;
    for (uint64_t i = 0; i < len; ++i) {
        if (f[i] == r[i]) {
            res = (res << 1) | f[i];
        }
    }

    free(f);
    free(r);
    return res;
}
