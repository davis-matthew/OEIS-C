/*
 * A094212  Duplicate of A070832.
 * https://oeis.org/A094212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = Sum_{k=0..n} binomial(8*n,8*k) (duplicate of A070832).
 *            Computed by the recurrence a(n) = 136*a(n-1) + 32880*a(n-2) -
 *            552704*a(n-3) - 65536*a(n-4) for n >= 5, with a(0)=1, a(1)=2,
 *            a(2)=12872, a(3)=1470944, a(4)=622116992
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 6, i128 n <= 6,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t v[5];
    v[0] = (seq_t)1;
    v[1] = (seq_t)2;
    v[2] = (seq_t)12872;
    v[3] = (seq_t)1470944;
    v[4] = (seq_t)622116992;
    if (n < 5) return v[(int)n];
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)136 * v[4]
                   + (seq_t)32880 * v[3]
                   - (seq_t)552704 * v[2]
                   - (seq_t)65536 * v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = next;
    }
    return v[4];
}
