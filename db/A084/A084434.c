/*
 * A084434  Numbers whose digit permutations have GCD > 1.
 * https://oeis.org/A084434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   all_even_digits, digit_frequencies, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A084434: numbers whose digit permutations have GCD > 1.
   The algorithm checks each integer in order against the established
   classification (multiples of 3, all even digits, identical digits,
   digits only in {5,0} or {7,0}, or length divisible by 6 with digits {1,8}). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/all_even_digits.h"
#include "../utils/digit_frequencies.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 2;

    if (n == 0)
        return 0;

    while (cache_len < n) {
        seq_t x = next_candidate;
        next_candidate = next_candidate + 1;

        bool ok = false;

        if ((x % 3) == 0)
            ok = true;
        else if (all_even_digits(x))
            ok = true;
        else {
            unsigned int freq[10] = {0};
            digit_frequencies(x, freq);
            int len = digit_len(x);
            int distinct = 0;
            for (int d = 0; d < 10; ++d)
                if (freq[d] > 0) {
                    ++distinct;
                    if (distinct > 1) break;
                }
            if (distinct == 1)
                ok = true;
            else if (freq[5] + freq[0] == (unsigned)len && freq[5] > 0)
                ok = true;
            else if (freq[7] + freq[0] == (unsigned)len && freq[7] > 0)
                ok = true;
            else if (len % 6 == 0 && (freq[1] + freq[8] == (unsigned)len))
                ok = true;
        }

        if (ok) {
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache)
                    abort();
            }
            cache[cache_len++] = x;
        }
    }

    return cache[n - 1];
}
