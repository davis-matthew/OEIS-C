/*
 * A131954  a(n) = sum of digits of (n! + a(n-1)), with a(1)=1.
 * https://oeis.org/A131954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = Sum_digits(n! + a(n-1)), with a(1) = 1
 * Range:     u8 n <= 51, u16 n <= 4807, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Base for the hand‑rolled big integer representation of n!. */
#define BIGBASE 1000000000ULL   /* 10^9, convenient 9 decimal digits per limb */

seq_t a(idx_t n)
{
    /* Cache of previously computed a(k) values; 1‑based indexing. */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;          /* Largest n that has been computed */

    /* Dynamic big integer for the factorial; stored in 32‑bit limbs of 9
       decimal digits each.  Using 32 bits keeps the carry < 2^32 for
       multiplication by k <= 10000. */
    static uint32_t big[5000];
    static size_t big_len = 1;          /* Current number of limbs */

    /* Ensure the cache is large enough for the requested n (1‑based). */
    if (cache == NULL || cached_n < n) {
        size_t new_cap = (size_t)(n + 2);          /* +1 for 1‑based, +1 safety */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL)   /* Allocation failure – never expected. */
            return (seq_t)0;
        cache = new_cache;

        /* First‑time initialization. */
        if (cached_n == 0) {
            cache[1] = (seq_t)1;        /* a(1) = 1 */
            cached_n = 1;
            big[0] = (uint32_t)1;      /* 1! = 1 */
            big_len = 1;
        }
    }

    /* If the required value has already been computed, return it. */
    if (n <= cached_n)
        return cache[n];

    /* Compute a(k) for all k = cached_n+1 … n, extending the factorial. */
    for (idx_t k = cached_n + 1; k <= n; ++k) {
        /* Multiply the current factorial by k. */
        uint64_t carry = 0;
        for (size_t i = 0; i < big_len; ++i) {
            uint64_t prod = (uint64_t)big[i] * (uint64_t)k + carry;
            big[i] = (uint32_t)(prod % BIGBASE);
            carry  = prod / BIGBASE;
        }
        while (carry) {
            big[big_len++] = (uint32_t)(carry % BIGBASE);
            carry /= BIGBASE;
        }

        /* Copy factorial into a temporary array so we can add a(k-1). */
        uint32_t tmp[5000];
        memcpy(tmp, big, big_len * sizeof(uint32_t));
        size_t tmp_len = big_len;

        /* Add a(k-1) to the temporary big integer. */
        uint64_t add = (uint64_t)cache[k - 1];
        size_t pos = 0;
        while (add && pos < tmp_len) {
            uint64_t sum = (uint64_t)tmp[pos] + add;
            tmp[pos] = (uint32_t)(sum % BIGBASE);
            add = sum / BIGBASE;
            ++pos;
        }
        while (add) {
            tmp[tmp_len++] = (uint32_t)(add % BIGBASE);
            add /= BIGBASE;
        }

        /* Sum the decimal digits of the resulting number. */
        uint64_t digit_sum = 0;
        /* Process most significant limb separately (no leading zeros). */
        if (tmp_len > 0) {
            uint32_t ms_limb = tmp[tmp_len - 1];
            while (ms_limb) {
                digit_sum += ms_limb % 10;
                ms_limb /= 10;
            }
        }
        /* All other limbs have exactly 9 decimal digits. */
        for (size_t idx = 0; idx + 1 < tmp_len; ++idx) {
            uint32_t limb = tmp[idx];
            for (int d = 0; d < 9; ++d) {
                digit_sum += limb % 10;
                limb /= 10;
            }
        }

        cache[k] = (seq_t)digit_sum;            /* Store result in cache */
    }

    cached_n = n;
    return cache[n];
}
