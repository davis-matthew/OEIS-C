/*
 * A062910  Non-palindromic number and its reversal are both multiples of 6.
 * https://oeis.org/A062910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 762, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for multiples of 6 that are non‑palindromic and whose reversal is also divisible by 6.
   a(1) = 24, a(2) = 42, a(3) = 48, ...  (OEIS A062910) */
#include <stdbool.h>

static inline seq_t reverse_digits(seq_t v)
{
    seq_t r = 0;
    while (v > 0) {
        r = r * 10 + (v % 10);
        v /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for n=0, but safe return */

    idx_t count = 0;
    seq_t candidate = 6;            /* first positive multiple of 6 */

    for (;; candidate += 6) {
        seq_t rev = reverse_digits(candidate);
        if (rev == candidate)           /* palindrome -> skip */
            continue;
        if (rev % 6 == 0) {             /* reversal is multiple of 6 */
            ++count;
            if (count == n)
                return candidate;
        }
    }
}
