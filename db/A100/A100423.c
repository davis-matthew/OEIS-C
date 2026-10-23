/*
 * A100423  Numbers n such that 30*n+{1,7,11,13,17,19,29} are all prime.
 * https://oeis.org/A100423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth integer k (k ≡ 6 (mod 7)) such that 30*k +
 *            {1,7,11,13,17,19,29} are all prime
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    uint64_t count = 0;
    uint64_t k = 6;                  /* k ≡ 6 (mod 7) */
    const int offsets[7] = {1, 7, 11, 13, 17, 19, 29};

    while (count < n)
    {
        bool all_prime = true;
        for (int i = 0; i < 7; ++i)
        {
            uint64_t v = k * 30ULL + (uint64_t)offsets[i];
            if (!is_prime_uint64(v))
            {
                all_prime = false;
                break;
            }
        }
        if (all_prime)
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        k += 7;
    }
    /* Unreachable: n ≥ 1 guarantees a return */
    return 0;
}
