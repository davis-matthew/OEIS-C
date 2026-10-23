/*
 * A109448  Numbers n such that the concatenation of consecutive increasing
 *          numbers beginning with Harshad(n) and ending with Harshad(n+1) is
 *          a Harshad number; or n such that A112131(n) is a Harshad number.
 * https://oeis.org/A109448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A109448
 *  a(n) = the n‑th integer m such that:
 *  let H(x) be the smallest Harshad number ≥ x
 *  (i.e. an integer divisible by the sum of its decimal digits).
 *  If H(m) ≠ H(m+1), then concatenate all integers from H(m) to H(m+1)
 *  inclusive (in decimal) and check whether that huge decimal number
 *  is itself a Harshad number.  The m that satisfy this are the terms
 *  of the sequence.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the sum of decimal digits of an idx_t value as a uint64_t */
static inline uint64_t digit_sum_u64(idx_t x)
{
    uint64_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* Return the number of decimal digits of an idx_t value */
static inline int digit_len(idx_t x)
{
    int len = 0;
    do {
        ++len;
        x /= 10;
    } while (x > 0);
    return len;
}

/* Return the smallest harshad number ≥ val */
static idx_t next_harshad_ge(idx_t val)
{
    for (idx_t x = val; ; ++x) {
        uint64_t sum = digit_sum_u64(x);
        if (sum == 0) continue;            /* x > 0, so this never happens */
        if ((x % sum) == 0) return x;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t found = 0;
    /* candidate is the current integer m to test */
    for (idx_t candidate = 1; ; ++candidate) {
        idx_t h1 = next_harshad_ge(candidate);
        idx_t h2 = next_harshad_ge(candidate + 1);

        /* skip when the interval is empty (h1 == h2) */
        if (h1 == h2) continue;

        /* Sum of digits of the concatenation (sum of digit sums of each int) */
        uint64_t sum_digits = 0;
        for (idx_t x = h1; x <= h2; ++x)
            sum_digits += digit_sum_u64(x);

        if (sum_digits == 0) continue;  /* should not happen */

        /* Compute the concatenated number modulo sum_digits */
        uint64_t rem = 0;
        for (idx_t x = h1; x <= h2; ++x) {
            int len = digit_len(x);
            /* 10^len mod sum_digits */
            uint64_t pow10_mod = 1 % sum_digits;
            for (int i = 0; i < len; ++i)
                pow10_mod = (pow10_mod * 10) % sum_digits;
            rem = (rem * pow10_mod + (uint64_t)x) % sum_digits;
        }

        if (rem == 0) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
    }
}
