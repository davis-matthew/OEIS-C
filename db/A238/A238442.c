/*
 * A238442  Triangle read by rows demonstrating Euler's pentagonal theorem for
 *          the sum of divisors.
 * https://oeis.org/A238442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   T(n,k) = A057077(k-1)*(n == A001318(k) ? A001318(k) : A000203(n
 *            - A001318(k))), 1 <= k <= A235963(n); a(n) read by rows
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A238442_pent(uint64_t k)
{
    uint64_t m;
    if (k & 1) {
        m = (k + 1) / 2;
        return m * (3 * m - 1) / 2;
    }
    m = k / 2;
    return m * (3 * m + 1) / 2;
}

static seq_t A238442_sigma(uint64_t m)
{
    seq_t result = 1;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) {
        result *= ((seq_t)m + 1);
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t idx = (uint64_t)n;
    uint64_t r = 1;
    for (;;) {
        uint64_t cnt = 0;
        while (A238442_pent(cnt + 1) <= r) cnt++;
        if (idx <= cnt) {
            uint64_t k = idx;
            uint64_t P = A238442_pent(k);
            seq_t v;
            if (P == r) v = (seq_t)P;
            else v = A238442_sigma(r - P);
            return ((k - 1) & 2) ? -v : v;
        }
        idx -= cnt;
        r++;
    }
}
