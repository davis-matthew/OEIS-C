/*
 * A050657  Numbers n such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 6.
 * https://oeis.org/A050657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050657: Numbers n such that the count of distinct primes obtained by
 * changing exactly one decimal digit of n equals 6.  The function searches
 * candidates in increasing order and counts the required primes using
 * is_prime_uint64 from @../utils/is_prime_uint64.h. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"

static int count_one_digit_prime_variants(uint64_t n)
{
    /* Extract decimal digits of n (least–significant first). */
    uint8_t digits[20];
    int len = 0;
    uint64_t temp = n;
    while (temp > 0) {
        digits[len++] = temp % 10;
        temp /= 10;
    }
    if (len == 0) /* n == 0 is outside the sequence */
        return 0;

    /* Store primes found, deduplicate after each change. */
    uint64_t primes[81]; /* at most 9 positions × 9 replacements */
    int pc = 0;

    uint64_t weight = 1; /* 10^pos, pos=0 for least‑significant digit */
    for (int pos = 0; pos < len; ++pos) {
        uint8_t cur_d = digits[pos];
        for (uint8_t new_d = 0; new_d <= 9; ++new_d) {
            if (new_d == cur_d)
                continue;
            if (pos == len - 1 && new_d == 0) /* leading zero not allowed */
                continue;

            uint64_t candidate;
            if (new_d > cur_d)
                candidate = n + (uint64_t)(new_d - cur_d) * weight;
            else
                candidate = n - (uint64_t)(cur_d - new_d) * weight;

            if (!is_prime_uint64(candidate))
                continue;

            /* Avoid duplicates */
            bool seen = false;
            for (int j = 0; j < pc; ++j) {
                if (primes[j] == candidate) {
                    seen = true;
                    break;
                }
            }
            if (!seen)
                primes[pc++] = candidate;
        }
        weight *= 10; /* 10^pos for next higher digit */
    }
    return pc;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* offset is 1 */

    idx_t target = n;
    idx_t found = 0;
    uint64_t candidate = 23; /* smallest positive integer with 6 variants */

    while (found < target) {
        if (count_one_digit_prime_variants(candidate) == 6) {
            ++found;
            if (found == target)
                return (seq_t)candidate;
        }
        ++candidate;
    }
    return 0; /* unreachable */
}
