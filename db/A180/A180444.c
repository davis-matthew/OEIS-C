/*
 * A180444  a(n) equals the number of bispecial Sturmian words of length n,
 *          that is words which are prefix to two words of length n+1, and
 *          likewise suffix. Note that prefix and suffix are not independent,
 *          unless the word is also palindromic: see A000010.
 * https://oeis.org/A180444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n+1) - phi(n+2)
 * Range:     u8 n <= 147, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t phi_val = calc_phi(sn + 2);
    seq_t result = (seq_t)2 * (sn + 1) - phi_val;
    return result;
}
