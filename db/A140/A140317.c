/*
 * A140317  Smallest n-digit number divisible by n^2.
 * https://oeis.org/A140317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n^2*ceil(10^(n-1)/n^2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    // Convert index to sequence type
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;          // n^2
    seq_t p10 = pow10(n - 1);        // 10^(n-1)
    
    // Ceiling division: (p10 + n2 - 1) / n2
    seq_t q = (p10 + n2 - 1) / n2;
    return n2 * q;
}
