/*
 * A231562  Numbers n such that A031971(8490421583559688410706771261086*n) ==
 *          n (mod 8490421583559688410706771261086*n).
 * https://oeis.org/A231562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c*m(n), c = 39607528021345872635, m(n) = n-th positive
 *            integer coprime to 14 (1,3,5,9,11,13,15,...); pattern inferred
 *            from the listed terms, not proven
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 11, big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const unsigned res[6] = {1, 3, 5, 9, 11, 13};
    idx_t q = (n - 1) / 6;
    idx_t r = (n - 1) % 6;
    seq_t m = (seq_t)q * 14 + (seq_t)res[r];
    seq_t c = (seq_t)3960752802ULL * (seq_t)10000000000ULL + (seq_t)1345872635ULL;
    return c * m;
}
