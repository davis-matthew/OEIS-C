/*
 * A093805  Numbers n with property that sum of digits is prime and number of
 *          prime digits is prime.
 * https://oeis.org/A093805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 970 (2026-11-01)
 * Helpers:   digit_frequencies, digit_sum, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A093805: numbers n such that the decimal digit sum of n is prime
   and the count of prime digits (2,3,5,7) in n is also prime.
   Computed by brute‑force search. */
#include <stdbool.h>
#include "../utils/digit_frequencies.h"
#include "../utils/digit_sum.h"
#include "../utils/is_prime_seq.h"

static inline bool is_valid(seq_t x)
{
    /* digit sum must be prime */
    if (!is_prime_seq(digit_sum(x)))
        return false;

    unsigned int freq[10] = {0};
    digit_frequencies(x, freq);

    /* count prime digits */
    int prime_digit_count = freq[2] + freq[3] + freq[5] + freq[7];
    return is_prime_seq((seq_t)prime_digit_count);
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for the sequence, but safe return */
        return 0;

    seq_t cand = 1;      /* start testing from 2 onward */
    idx_t found = 0;

    while (1) {
        ++cand;
        if (is_valid(cand)) {
            ++found;
            if (found == n)
                return cand;
        }
    }
}
