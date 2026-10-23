/*
 * A165494  Iterations of k -> k+tau(k) from 2(2n-1)^2 until result not
 *          divisible by 4.
 * https://oeis.org/A165494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterations of k → k + τ(k) starting from 2(2n‑1)²
   until the current value of k is not divisible by 4. */

#include <stddef.h>
#include <stdint.h>

/* Return the number of positive divisors of k. */
static seq_t tau(seq_t k)
{
    if (k <= 1) return 1;
    seq_t result = 1;
    for (seq_t d = 2; d * d <= k; ++d)
    {
        seq_t exp = 0;
        while (k % d == 0) {
            k /= d;
            ++exp;
        }
        if (exp != 0)
            result *= (exp + 1);
    }
    if (k > 1)
        result *= 2;
    return result;
}

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t twon_minus1 = (seq_t)2 * nseq - 1;
    seq_t start = (seq_t)2 * twon_minus1 * twon_minus1;

    seq_t k = start;
    seq_t count = 0;

    for (;;)
    {
        k += tau(k);
        ++count;
        if (k % 4 != 0)
            break;
    }
    return count;
}
