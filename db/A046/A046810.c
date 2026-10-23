/*
 * A046810  Number of anagrams of n that are primes.
 * https://oeis.org/A046810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(n) = number of distinct prime anagrams of the decimal
 *            representation of n (leading zeros are not permitted)
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Recursive helper to generate all distinct permutations of the
   digits array, skipping those starting with 0.  When a full
   permutation is formed, the prime test is applied and the
   counter is incremented if the result is prime.  Duplicate digits
   are handled by skipping equal digits that have not yet been used
   at the current recursion level. */
static void
gen_perm(int pos, const uint8_t *digits, bool *used, int len,
         seq_t current, seq_t *cnt)
{
    if (pos == len) {
        if (is_prime_big(current))
            (*cnt)++;
        return;
    }

    for (int i = 0; i < len; i++) {
        if (used[i])
            continue;
        if (i > 0 && digits[i] == digits[i - 1] && !used[i - 1])
            continue;               /* skip duplicates */
        if (pos == 0 && digits[i] == 0)
            continue;               /* no leading zero */

        used[i] = true;
        gen_perm(pos + 1, digits, used, len,
                 current * 10 + digits[i], cnt);
        used[i] = false;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)            /* 1 has no prime anagrams, 0 undefined but
                             return 0 for safety */
        return 0;

    /* Convert n into its decimal digits (most significant first) */
    uint8_t digits[20];
    int d = 0;
    uint64_t tmp = (uint64_t)n;
    do {
        digits[d++] = tmp % 10;
        tmp /= 10;
    } while (tmp);

    /* Sort digits ascending to enable duplicate handling in recursion */
    for (int i = 0; i < d - 1; i++) {
        for (int j = i + 1; j < d; j++) {
            if (digits[i] > digits[j]) {
                uint8_t t = digits[i];
                digits[i] = digits[j];
                digits[j] = t;
            }
        }
    }

    bool used[20] = {0};
    seq_t count = 0;
    gen_perm(0, digits, used, d, 0, &count);

    return count;
}
