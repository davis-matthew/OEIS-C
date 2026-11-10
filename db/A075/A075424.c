/*
 * A075424  a(n) = A075423(A075423(n)).
 * https://oeis.org/A075424
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A075423(A075423(n)), where A075423(m) = rad(m) - 1, so
 *            a(n) = rad(rad(n) - 1) - 1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9827 (2026-11-07)
 * Helpers:   radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/radical.h"

seq_t a(idx_t n)
{
    seq_t m = radical(n) - 1;
    idx_t mi = (idx_t)m;
    if (mi == 0) return 0;
    return radical(mi) - 1;
}
