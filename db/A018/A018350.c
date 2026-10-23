/*
 * A018350  Divisors of 240.
 * https://oeis.org/A018350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th divisor of 240 in increasing order */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 240 = 2⁴ · 3¹ · 5¹, so its 20 divisors are
       2ᵃ·3ᵇ·5ᶜ with 0≤a≤4, 0≤b≤1, 0≤c≤1. */
    seq_t divs[20];
    size_t cnt = 0;

    for (idx_t a = 0; a <= 4; ++a) {
        seq_t val2 = (seq_t)1 << a;   /* 2ᵃ */
        for (idx_t b = 0; b <= 1; ++b) {
            seq_t val23 = val2 * (b ? (seq_t)3 : (seq_t)1);
            for (idx_t c = 0; c <= 1; ++c) {
                divs[cnt++] = val23 * (c ? (seq_t)5 : (seq_t)1);
            }
        }
    }

    /* Sort the 20 divisors in ascending order (simple bubble sort). */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (divs[i] > divs[j]) {
                seq_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }
        }
    }

    if (n == 0) {                 /* offset is 1, n should be >=1 */
        return (seq_t)0;
    }
    if (n <= cnt) {
        return divs[(size_t)(n - 1)];
    }
    /* For n larger than the number of divisors, return 0 (default). */
    return (seq_t)0;
}
