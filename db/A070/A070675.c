/*
 * A070675  Smallest m in range 2..n-1 such that m^10 == 1 mod n, or 1 if no
 *          such number exists.
 * https://oeis.org/A070675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A070675:  smallest m in 2..n‑1 such that m^10 ≡ 1 (mod n),
   returning 1 if no such m exists. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n = 1 or 2 the range is empty, answer is 1 */
    if (n <= 2)
        return (seq_t)1;

    seq_t mod = (seq_t)n;

    for (seq_t m = 2; m < mod; ++m)
    {
        seq_t res = 1;
        /* Compute m^10 mod n by repeated multiplication */
        for (int i = 0; i < 10; ++i)
            res = (res * m) % mod;

        if (res == 1)
            return m;
    }
    return (seq_t)1;
}
