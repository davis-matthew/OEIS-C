/*
 * A067103  a(n) = floor(X/Y), where X = concatenation of cubes and Y =
 *          concatenation of natural numbers.
 * https://oeis.org/A067103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor( concat(k^3, 1≤k≤n) / concat(k, 1≤k≤n) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdio.h>

static int digits_of_uint64(uint64_t x)
{
    /* x≥1 */
    int d = 0;
    while (x) { x /= 10; ++d; }
    return d ? d : 1;
}

seq_t a(idx_t n)
{
    /* Pre‑compute powers of ten up to 10^20 (enough for all digit lengths that occur). */
    static seq_t pow10[21];
    static int pow10_ready = 0;
    if (!pow10_ready) {
        pow10[0] = 1;
        for (int i = 1; i < 21; ++i) pow10[i] = pow10[i-1] * 10;
        pow10_ready = 1;
    }

    /* Build Y = concatenation of 1..n */
    seq_t Y = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kseq = (seq_t)k;
        int dk = digits_of_uint64((uint64_t)k);
        Y = Y * pow10[dk] + kseq;
    }

    /* Divide the decimal representation of X = concat(k³, 1≤k≤n) by Y
       using a long‑division algorithm that never stores X as a whole
       (so it works even when X does not fit in seq_t). */
    seq_t quotient = 0;
    seq_t remainder = 0;

    char buf[32];  /* buffer for decimal representation of a cube */

    for (idx_t k = 1; k <= n; ++k) {
        unsigned long long cube = (unsigned long long)k *
                                  (unsigned long long)k *
                                  (unsigned long long)k;
        int len = snprintf(buf, sizeof(buf), "%llu", cube);
        for (int pos = 0; pos < len; ++pos) {
            int digit = buf[pos] - '0';
            remainder = remainder * 10 + (seq_t)digit;
            seq_t qdig = remainder / Y;
            remainder = remainder % Y;
            quotient = quotient * 10 + qdig;
        }
    }

    return quotient;
}
