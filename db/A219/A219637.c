/*
 * A219637  Numbers that occur twice in A219641.
 * https://oeis.org/A219637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th value v such that exactly two m >= 0 have m -
 *            A007895(m) = v (A219641 value counts); computed by a linear
 *            scan, since A219641 is nondecreasing
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 8448 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static uint64_t A219637_fibs[92];
static int A219637_flen = 0;

static void A219637_init(void)
{
    if (A219637_flen) return;
    A219637_fibs[0] = 1;
    A219637_fibs[1] = 2;
    A219637_flen = 2;
    while (A219637_flen < 90) {
        A219637_fibs[A219637_flen] = A219637_fibs[A219637_flen - 1] + A219637_fibs[A219637_flen - 2];
        A219637_flen++;
    }
}

static uint64_t A219637_f(uint64_t m)
{
    int i = A219637_flen - 1;
    uint64_t ones = 0;
    while (m > 0) {
        while (i > 0 && A219637_fibs[i] > m) i--;
        m -= A219637_fibs[i];
        ones++;
    }
    return ones;
}

seq_t a(idx_t n)
{
    A219637_init();
    uint64_t m = 0;
    uint64_t prev = 0;      /* f(0) = 0 */
    uint64_t run = 1;
    idx_t cnt = 0;
    for (;;) {
        m++;
        uint64_t v = m - A219637_f(m);
        if (v == prev) {
            run++;
        } else {
            if (run == 2) {
                cnt++;
                if (cnt == n) return (seq_t)prev;
            }
            prev = v;
            run = 1;
        }
    }
}
