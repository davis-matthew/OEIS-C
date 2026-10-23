/*
 * A025695  Index of 2^n within the sequence of the numbers of the form
 *          2^i*10^j.
 * https://oeis.org/A025695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=0..n} ceil(k * log10(2))
 * Range:     u8 n <= 38, u16 n <= 657, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic with a rational approximation
 * of log10(2).  NUM/DEN ≈ log10(2) with DEN = 10^18.
 * For all k ≤ n (n ≤ 10000 for performance) the error is < 1/k, 
 * so ceil(k * log10(2)) = (k*NUM + DEN - 1)/DEN. */

#include <stdint.h>

static const unsigned long long NUM_U64 = 301029995663981196ULL;   /* floor(log10(2)*10^18)+1 */
static const unsigned long long DEN_U64 = 1000000000000000000ULL;   /* 10^18 */

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* compute ceil(k * NUM/DEN) */
        __uint128_t prod = (__uint128_t)k * NUM_U64;
        uint64_t ceil_k = (uint64_t)((prod + DEN_U64 - 1) / DEN_U64);
        sum += (seq_t)ceil_k;
    }

    return sum + (seq_t)1;   /* a(0) = 1 */
}
