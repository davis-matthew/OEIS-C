/*
 * A182689  a(n) = the smallest 2-digit number with exactly n divisors, a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A182689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = smallest 2‑digit integer having exactly n divisors; a(n)
 *            = 0 if no such integer exists
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count divisors of a small unsigned integer (fits in uint64_t). */
static uint64_t divisor_count(uint64_t x)
{
    uint64_t cnt = 1;
    uint64_t tmp = x;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            cnt *= (uint64_t)(exp + 1);
        }
    }
    if (tmp > 1) cnt *= 2;
    return cnt;
}

/* Pre‑compute the smallest two‑digit number for each divisor count 1..12. */
static void build_best(uint64_t best[13])
{
    for (int i = 1; i <= 12; ++i) best[i] = 0;

    for (uint64_t m = 10; m <= 99; ++m) {
        uint64_t d = divisor_count(m);
        if (d >= 1 && d <= 12) {
            if (best[(int)d] == 0 || m < best[(int)d])
                best[(int)d] = m;
        }
    }
}

/* Sequence A182689: smallest 2‑digit number with exactly n divisors. */
seq_t a(idx_t n)
{
    if (n < 1 || n > 12) return (seq_t)0;

    static bool initialized = false;
    static uint64_t best[13];

    if (!initialized) {
        build_best(best);
        initialized = true;
    }

    return (seq_t)best[(int)n];
}
