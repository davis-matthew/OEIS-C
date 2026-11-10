/*
 * A211317  a(n) = A211316(2n+1).
 * https://oeis.org/A211317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = A211316(2n+1); A211316(m) = m*(p+1)/(3p) for p the
 *            smallest prime divisor of m with p≡2 (mod 3); else m/3 if 3|m;
 *            else (m-1)/3
 * Range:     u8 n <= 7, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 74 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A211316_a(uint64_t n)
{
    if (n < 2) {
        return 0;
    }
    uint64_t p = 0;
    uint64_t temp = n;
    for (uint64_t i = 2; i <= temp / i; ++i) {
        if (temp % i == 0) {
            if (i % 3 == 2) {
                p = i;
                break;
            }
            do {
                temp /= i;
            } while (temp % i == 0);
        }
    }
    if (p == 0 && temp > 1 && temp % 3 == 2) {
        p = temp;
    }
    seq_t N = (seq_t)n;
    if (p != 0) {
        seq_t num = N * (((seq_t)p) + 1);
        seq_t den = (seq_t)3 * (seq_t)p;
        return num / den;
    } else if (n % 3 == 0) {
        return N / 3;
    } else {
        return (N - 1) / 3;
    }
}

seq_t a(idx_t n)
{
    return A211316_a((uint64_t)n * 2 + 1);
}
