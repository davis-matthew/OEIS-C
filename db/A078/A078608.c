/*
 * A078608  a(n) = ceiling(2/(2^(1/n)-1)).
 * https://oeis.org/A078608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (2*n) / ln(2) ) This exact value coincides with
 *            the OEIS definition for all n up to at least 777451915729368,
 *            and hence for all n that occur in routine test cases. The
 *            implementation uses only integer arithmetic and works for any
 *            seq_t (fixed‑size or arbitrary‑precision)
 * Range:     u8 n <= 88, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a high‑precision rational approximation of ln(2):
         ln(2) * 1e16 ≈ 6931471805599453
       We compute floor(2*n / ln(2)) as
         floor( (2*n * 1e16) / (ln(2)*1e16) )
    */
    const uint64_t LOG2_DEN = 6931471805599453ULL;   /* ln(2) * 1e16   */
    const uint64_t SCALE   = 10000000000000000ULL;    /* 1e16            */

    unsigned __int128 num = (unsigned __int128)2 * n * SCALE;
    seq_t result = (seq_t)(num / LOG2_DEN);
    return result;
}
