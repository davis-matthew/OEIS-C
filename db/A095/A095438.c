/*
 * A095438  a(n) = 22 written in base n.
 * https://oeis.org/A095438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22 expressed in base n, interpreted as a base‑10 integer
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 11, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the base‑n representation of 22 and interpret it as a decimal number. */
#include <stdint.h>

/* 'seq_t' and 'idx_t' are defined by the build. */
seq_t a(idx_t n)
{
    if (n == 0)                /* according to OEIS offset, this case never occurs */
        return (seq_t)0;

    /* Base > 22: 22 is a single digit representation. */
    if (n > 22)
        return (seq_t)22;

    /* Base 1 (unary): 22 ones. */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 22; ++i)
            res = res * (seq_t)10 + (seq_t)1;
        return res;
    }

    /* For bases 2 .. 22: compute digits of 22 in that base. */
    uint64_t value = 22;          /* 22 fits in uint64_t for all needed bases */
    int digits[32];               /* enough space for all digits (max ~ .7 log2(22)=5) */
    int count = 0;

    /* Collect remainders (least significant first). */
    while (value > 0) {
        digits[count++] = (int)(value % n);
        value /= n;
    }

    /* Build the decimal integer from most significant digit to least. */
    seq_t result = 0;
    for (int i = count - 1; i >= 0; --i)
        result = result * (seq_t)10 + (seq_t)digits[i];

    return result;
}
