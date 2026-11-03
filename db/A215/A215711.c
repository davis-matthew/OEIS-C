/*
 * A215711  Expansion of a(q) * b(q)^3 in powers of q where a(), b() are cubic
 *          AGM theta functions.
 * https://oeis.org/A215711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; for n > 0: a(n) = -3 * σ₃(n) + (3|n ? 243 * σ₃(n/3) :
 *            0), where σ₃(m) = Σ_{d|m} d³
 * Range:     i8 n <= 2, i16 n <= 17, i32 n <= 683, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Validated: standard, pass, 52 terms through n = 2474 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* harmless inclusion */

static seq_t sigma3(idx_t m)
{
    if (m == 0) return 0;
    seq_t sum = 0;
    for (idx_t d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            seq_t dseq = (seq_t)d;
            seq_t d3 = dseq * dseq * dseq;
            sum += d3;
            idx_t e = m / d;
            if (e != d) {
                seq_t eseq = (seq_t)e;
                seq_t e3 = eseq * eseq * eseq;
                sum += e3;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t sig = sigma3(n);
    seq_t result = -3 * sig;
    if (n % 3 == 0) {
        seq_t sig3 = sigma3(n / 3);
        result += 243 * sig3; /* 243 = 3 * 81 */
    }
    return result;
}
