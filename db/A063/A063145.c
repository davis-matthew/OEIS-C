/*
 * A063145  Composite numbers not divisible by 2 which in base 4 contain their
 *          largest proper factor as a substring.
 * https://oeis.org/A063145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 22, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   _smallest_prime_factor, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063145: composite odd numbers whose base‑4 representation
   contains the base‑4 representation of their largest proper factor
   as a contiguous substring.  The function enumerates such numbers
   in ascending order, caching results so that random access to the
   sequence is O(1).  The algorithm is:
        iterate through odd integers,
        reject primes,
        let lp = n / sp where sp is the smallest prime factor,
        convert n and lp to base‑4 (most significant digit first),
        test if lp_digits appears as a substring of n_digits.
   All arithmetic is performed with uint64_t, which is sufficient
   for the first many thousand terms (the values fit easily in a 64‑bit
   integer).  The function works for any seq_t type that is an unsigned
   integer (fixed‑size or arbitrary precision). */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t last_checked = 1;    /* last integer examined */

    /* Produce terms until we have at least n of them */
    while (cache_len < n) {
        ++last_checked;
        /* Skip even numbers */
        if ((last_checked & 1ULL) == 0)
            continue;

        uint64_t sp = _smallest_prime_factor(last_checked);
        if (sp == last_checked)            /* prime, skip */
            continue;

        uint64_t lp = last_checked / sp;   /* largest proper divisor */

        /* Convert last_checked to base‑4 digits (most significant first) */
        uint8_t n_digits[64], lp_digits[64];
        size_t n_len = 0, lp_len = 0;
        uint64_t tmp = last_checked;
        do {
            n_digits[n_len++] = (uint8_t)(tmp & 3);
            tmp >>= 2;
        } while (tmp);
        /* reverse to get msb‑first */
        for (size_t i = 0; i < n_len / 2; ++i) {
            uint8_t d = n_digits[i];
            n_digits[i] = n_digits[n_len - 1 - i];
            n_digits[n_len - 1 - i] = d;
        }

        tmp = lp;
        do {
            lp_digits[lp_len++] = (uint8_t)(tmp & 3);
            tmp >>= 2;
        } while (tmp);
        for (size_t i = 0; i < lp_len / 2; ++i) {
            uint8_t d = lp_digits[i];
            lp_digits[i] = lp_digits[lp_len - 1 - i];
            lp_digits[lp_len - 1 - i] = d;
        }

        /* Search lp_digits inside n_digits (as a contiguous substring) */
        bool found = false;
        if (lp_len <= n_len) {
            for (size_t i = 0; i <= n_len - lp_len && !found; ++i) {
                size_t j;
                for (j = 0; j < lp_len; ++j) {
                    if (n_digits[i + j] != lp_digits[j])
                        break;
                }
                if (j == lp_len)
                    found = true;
            }
        }

        if (found) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
