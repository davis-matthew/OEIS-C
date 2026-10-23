/*
 * A166061  19-rough numbers: positive integers that have no prime factors
 *          less than 19.
 * https://oeis.org/A166061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A166061: 19‑rough numbers – positive integers with no prime factor < 19.
    The sequence is the list of integers coprime to 510510 = 2·3·5·7·11·13·17.
    a(n) is the n‑th such number, with a(1)=1.  */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Primes less than 19 */
static const uint32_t _primes_19[] = {2, 3, 5, 7, 11, 13, 17};
#define _NUM_P19 7

/* Count how many integers ≤ x are coprime to 510510 using inclusion–exclusion. */
static seq_t _count_coprime_to_510510(seq_t x)
{
    /* Use a signed 64‑bit integer to avoid underflow during inclusion–exclusion. */
    int64_t res = (int64_t)x;

    /* Iterate over all non‑empty subsets of the 7 primes. */
    for (int mask = 1; mask < (1 << _NUM_P19); ++mask)
    {
        seq_t prod = 1;
        for (int j = 0; j < _NUM_P19; ++j)
        {
            if (mask & (1 << j))
                prod *= (seq_t)_primes_19[j];
        }
        seq_t term = x / prod;
        if (__builtin_popcount(mask) & 1)
            res -= (int64_t)term;
        else
            res += (int64_t)term;
    }
    return (seq_t)res;
}

/* Return the n‑th 19‑rough number. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* Undefined for n = 0, but harmless. */

    const seq_t N = (seq_t)n;   /* Make N exactly the type of the sequence value. */
    seq_t low = 1;
    seq_t high = N * 6;         /* Initial upper bound, safe for small n. */
    if (high < N) high = N << 3;    /* Overflow guard. */
    if (high == 0) high = 1;

    /* Increase the upper bound until it surely contains the nth number. */
    while (_count_coprime_to_510510(high) < N)
        high <<= 1;

    /* Binary search for the smallest x with at least N coprime numbers ≤ x. */
    while (low < high)
    {
        seq_t mid = (low + high) / 2;
        if (_count_coprime_to_510510(mid) >= N)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}
