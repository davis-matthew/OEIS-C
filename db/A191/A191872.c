/*
 * A191872  a(n) is the smallest multiple of n such that the sum of the square
 *          of the decimal digits of a(n) is divisible by n.
 * https://oeis.org/A191872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 59, u32 n <= 386, u64 n <= 467,
 *            u128 n <= 467, big any n
 * Helpers:   digit_frequencies, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A191872
 *  a(n) = smallest multiple of n such that the sum of the squares of the decimal digits of a(n) is divisible by n.
 *  Offset: 1
 *
 *  Definition: let S(x) = Σ d_i^2 over the decimal digits of x.
 *          a(n) = min{ k∗n | k≥1, S(k∗n) ≡ 0 (mod n) }.
 */

#include <stdbool.h>
#include "../utils/digit_frequencies.h"
#include "../utils/seq_mul.h"

static uint64_t digit_square_sum_seq(seq_t x)
{
    unsigned int cnt[10] = {0};
    digit_frequencies(x, cnt);
    uint64_t sum = 0;
    for (int d = 0; d <= 9; ++d)
        sum += (uint64_t)cnt[d] * d * d;
    return sum;
}

seq_t a(idx_t n)
{
    /* Handle the offset: sequence starts at n=1. For n = 0 we return 0. */
    if (n == 0) return 0;

    /* Convert n to seq_t for multiplication, and to uint64_t for modulo checks. */
    seq_t N = (seq_t)n;
    uint64_t n_u = (uint64_t)n;

    idx_t k = 1;
    while (true)
    {
        seq_t prod = seq_mul(N, (seq_t)k);
        uint64_t sum = digit_square_sum_seq(prod);
        if (sum % n_u == 0)
            return prod;

        ++k;
    }
}
