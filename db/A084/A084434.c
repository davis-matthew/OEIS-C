/*
 * A084434  Numbers whose digit permutations have GCD > 1.
 * https://oeis.org/A084434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   digit_frequencies, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A084434: numbers whose digit permutations have GCD > 1.
   The algorithm checks each integer in order against the established
   classification (multiples of 3, all even digits, identical digits,
   digits only in {5,0} or {7,0}, or length divisible by 6 with digits {1,8}). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_frequencies.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Test whether all decimal digits of x are even. */
static inline bool all_even_digits(seq_t x)
{
    if (x == 0)           /* 0 is considered to have only even digits */
        return true;

    while (x != 0) {
        seq_t dig   = x % 10;     /* least significant decimal digit   */
        if ((dig % 2) != 0)       /* odd digit found                    */
            return false;
        x /= 10;                  /* move to next digit                  */
    }
    return true;
}

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
            int len = num_digits(x);
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
                cache = mem_resize(seq_t, cache, cache_cap);
                if (!cache)
                    abort();
            }
            cache[cache_len++] = x;
        }
    }

    return cache[n - 1];
}
