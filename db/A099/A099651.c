/*
 * A099651  For each of the C(10,2) = 45 pairs of decimal digits, record the
 *          smallest prime containing only these digits (if one exists); sort.
 * https://oeis.org/A099651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   bitcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search:  For each unordered pair of decimal digits {i,j} (i<j) find the
   smallest prime that uses only those two digits (each of them at least once).
   Collect all such primes, sort ascending, and return the nth element
   (offset 1).  If n exceeds the number of such pairs, return 0. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/bitcount.h"

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t seq_arr[24];
    static size_t seq_len = 0;

    if (!init)
    {
        bool found[10][10] = {0};
        seq_t primes[10][10] = {0};
        size_t found_cnt = 0;
        const uint64_t LIMIT = 200000;   /* far above the largest needed value */

        for (uint64_t cand = 2; found_cnt < 24 && cand <= LIMIT; ++cand)
        {
            if (!is_prime_uint64(cand))
                continue;

            uint64_t tmp = cand;
            int mask = 0;
            while (tmp > 0)
            {
                int d = tmp % 10;
                mask |= 1 << d;
                tmp /= 10;
            }

            if (bitcount(mask) != 2)
                continue;

            int d1 = -1, d2 = -1;
            for (int d = 0; d < 10; ++d)
            {
                if (mask & (1 << d))
                {
                    if (d1 == -1)
                        d1 = d;
                    else
                        d2 = d;
                }
            }

            if (d1 < d2 && !found[d1][d2])
            {
                found[d1][d2] = true;
                primes[d1][d2] = (seq_t)cand;
                ++found_cnt;
            }
        }

        size_t pos = 0;
        for (int i = 0; i < 10; ++i)
            for (int j = i + 1; j < 10; ++j)
                if (found[i][j])
                    seq_arr[pos++] = primes[i][j];

        /* simple insertion sort – array is tiny (24 elements) */
        for (size_t i = 0; i < pos; ++i)
            for (size_t j = i + 1; j < pos; ++j)
                if (seq_arr[j] < seq_arr[i])
                {
                    seq_t tmp = seq_arr[i];
                    seq_arr[i] = seq_arr[j];
                    seq_arr[j] = tmp;
                }

        seq_len = pos;
        init = true;
    }

    if (n == 0 || (size_t)(n - 1) >= seq_len)
        return (seq_t)0;
    return seq_arr[(size_t)(n - 1)];
}
