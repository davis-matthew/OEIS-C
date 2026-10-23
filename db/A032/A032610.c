/*
 * A032610  Concatenation of n and n + 5 or {n,n+5}.
 * https://oeis.org/A032610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, n+5) = n*10^{digits(n+5)} + (n+5)
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    idx_t m = n + 5;
    int len = digit_count((uint64_t)m);
    seq_t pow = pow10((idx_t)len);
    seq_t res = (seq_t)n * pow + (seq_t)m;
    return res;
}
