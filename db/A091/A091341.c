/*
 * A091341  Natural numbers written out with their digits grouped in sets of 5
 *          (leading zeros omitted).
 * https://oeis.org/A091341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = integer formed by concatenating 5 consecutive digits *
 *            starting at position 5*(n-1) in the infinite decimal * expansion
 *            of the natural numbers 1,2,3,…, * with any leading zeros omitted
 * Range:     u8 none, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 52 terms through n = 9596 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned get_digit_from_position(idx_t pos)
{
    /* Determine the digit at global position 'pos' of the infinite
     * decimal string 123456789101112… .  The function uses 64‑bit
     * arithmetic which suffices for the required range (n ≤ 10 000). */
    unsigned long long remaining = (unsigned long long)pos;
    unsigned long long pow10 = 1ULL;          /* 10^(d-1)  */
    unsigned long long count_d = 9ULL;        /* numbers with d digits */
    unsigned long long d = 1ULL;              /* current digit length */

    while (remaining >= count_d * d) {
        remaining -= count_d * d;
        ++d;
        pow10 *= 10ULL;
        count_d = 9ULL * pow10;              /* 9·10^(d-1) */
    }

    unsigned long long num_index      = remaining / d;
    unsigned long long digit_offset   = remaining % d;
    unsigned long long number        = pow10 + num_index;

    unsigned long long divisor = 1ULL;
    unsigned long long zeros   = d - 1 - digit_offset;
    while (zeros--) divisor *= 10ULL;

    return (unsigned)((number / divisor) % 10ULL);
}

seq_t a(idx_t n)
{
    idx_t pos_start = (idx_t)5 * (n - 1);
    seq_t res = 0;

    for (int i = 0; i < 5; ++i) {
        unsigned digit = get_digit_from_position(pos_start + i);
        res = res * (seq_t)10 + (seq_t)digit;
    }

    return res;
}
