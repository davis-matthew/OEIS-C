/*
 * A062914  Non-palindromic number and its reversal are both multiples of 15.
 * https://oeis.org/A062914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 699, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 56 terms through n = 4802 (2026-11-01)
 * Helpers:   is_palindrome_base, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062914: Non‑palindromic numbers whose reverse is also a multiple of 15.
   The function generates terms by brute force search. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    /* offset = 1; so a(1) is the first valid number */
    if (n == 0) return 0;      /* undefined for n = 0, harmless */

    seq_t x = 0;      /* current candidate */
    idx_t count = 0;  /* number of valid terms found so far */

    for (;;) {
        ++x;                                     /* next integer */
        if (x % 15 != 0) continue;               /* number itself must be multiple of 15 */

        seq_t rev = reverse_digits(x);           /* reversed decimal representation */
        if (rev % 15 != 0) continue;             /* reverse must also be multiple of 15 */

        if (is_palindrome_base(x, 10)) continue;      /* exclude palindromes */

        ++count;                                 /* valid term found */
        if (count == n) return x;                /* return the requested term */
    }
}
