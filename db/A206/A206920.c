/*
 * A206920  Sum of the first n binary palindromes; a(n) = Sum_{k=1..n}
 *          A006995(k).
 * https://oeis.org/A206920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..n} A006995(k) (sum of the first n binary
 *            palindromes)
 * Range:     u8 n <= 13, u16 n <= 92, u32 n <= 3777, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t reverse_bits(seq_t x, int len)
{
    seq_t rev = 0;
    while (len-- > 0) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t remaining = n;

    /* Length 1 palindromes: 0 and 1 */
    if (remaining > 0) {
        sum += (seq_t)0;          /* 0 */
        --remaining;
        if (remaining == 0) return sum;
    }
    if (remaining > 0) {
        sum += (seq_t)1;          /* 1 */
        --remaining;
        if (remaining == 0) return sum;
    }

    /* Length >= 2 */
    for (int L = 2; remaining > 0; ++L) {
        int h = (L + 1) / 2;                 /* number of bits in the 'half' */
        seq_t start = ((seq_t)1) << (h - 1); /* first half with leading 1 */
        seq_t end   = ((seq_t)1) << h;       /* exclusive upper bound */

        for (seq_t i = start; i < end && remaining > 0; ++i) {
            seq_t pal;
            if ((L & 1) == 0) {                  /* even length */
                seq_t rev = reverse_bits(i, h);
                pal = (i << h) | rev;
            } else {                              /* odd length */
                seq_t rev = reverse_bits(i >> 1, h - 1);
                pal = (i << (h - 1)) | rev;
            }
            sum += pal;
            --remaining;
        }
    }
    return sum;
}
