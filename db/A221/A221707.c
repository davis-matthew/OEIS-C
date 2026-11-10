/*
 * A221707  Numbers n such that A216377(n) = 1.
 * https://oeis.org/A221707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 2 whose n! has leading base-m digit 1, i.e.
 *            frac(log_m(m!)) < log_m(2); found by a linear scan with a
 *            running sum of logs (A216377(m) = 1)
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 988 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    long double sum = 0.0L;
    const long double ln2 = logl(2.0L);
    idx_t cnt = 0;
    for (idx_t m = 2;; ++m) {
        long double lm = logl((long double)m);
        sum += lm;
        int ok;
        if (m == 2) {
            ok = 1;  /* 2! = 10 in base 2 */
        } else {
            long double k = floorl(sum / lm);
            long double t = sum - k * lm;
            ok = (t < ln2 - 1e-9L);
        }
        if (ok) {
            ++cnt;
            if (cnt == n)
                return (seq_t)m;
        }
    }
}
