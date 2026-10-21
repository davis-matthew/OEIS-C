/*
 * A221011  The hyper-Wiener index of the nanostar dendrimer G[n], defined
 *          pictorially in the Nadjafi-Arani et al. reference.
 * https://oeis.org/A221011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1095-2^(n-2)*33885+4^(n-2)*(15876*n^2-69930*n+118080)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 22, u128 n <= 53,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)159;

    idx_t exp = n - 2;

    seq_t t2 = pow2(exp);          // 2^(n-2)
    seq_t t4 = pow4(exp);          // 4^(n-2)

    seq_t tn = (seq_t)n;

    seq_t poly = ((seq_t)15876 * tn * tn) - ((seq_t)69930 * tn) + 118080;
    seq_t term1 = t4 * poly;       // 4^(n-2) * poly
    seq_t term2 = t2 * 33885;      // 2^(n-2) * 33885

    seq_t result = term1 + 1095;
    result = result - term2;

    return result;
}
