/*
 * A118595  Palindromes in base 4 (written in base 4).
 * https://oeis.org/A118595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   build_decimal_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all integers whose base‑4 representation is a
   palindrome. Each term is the decimal value of that palindrome
   (e.g. base‑4 101 is 101 decimal). */

#include <stdbool.h>
#include "../utils/build_decimal_palindrome.h"

/* a(n) = nth palindrome of base‑4 digits interpreted as decimal. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */

    idx_t count = 0;

    for (idx_t len = 1; ; ++len) {
        int half = (int)((len + 1) / 2);
        seq_t low, high;

        if (len == 1) {             /* single‑digit palindromes may start with 0 */
            low  = 0;
            high = 3;
        } else {
            /* low  = 4^(half‑1)   (most significant digit cannot be 0) */
            low  = ((seq_t)1) << (2 * (half - 1));
            /* high = 4^half - 1   (maximum value with 'half' digits) */
            high = ((((seq_t)1) << (2 * half)) - 1);
        }

        for (seq_t hi = low; hi <= high; ++hi) {
            seq_t val = build_decimal_palindrome(hi, half, (len & 1) != 0);
            ++count;
            if (count == n) return val;
        }
    }
}
