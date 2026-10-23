/*
 * A115701  Cubes whose digit reversal is the product of 2 palindromes greater
 *          than 1.
 * https://oeis.org/A115701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth cube m^3 such that rev_decimal(m^3) equals a
 *            product of two palindromes greater than 1
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   floor_sqrt, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine uses the supplied utilities for reversing digits and
   computing integer square roots.  No other external symbols are
   required. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"   /* seq_t reverse_digits(seq_t x); */
#include "../utils/floor_sqrt.h"       /* seq_t floor_sqrt(seq_t x); */

/* Return true if x is a palindrome in base 10. */
static bool is_palindrome(seq_t x)
{
    return x == reverse_digits(x);
}

/* Return true if rev can be factored into two palindromes > 1. */
static bool passes_palproduct(seq_t rev)
{
    seq_t sqrt_rev = floor_sqrt(rev);
    for (seq_t p = 2; p <= sqrt_rev; ++p) {
        if (rev % p != 0) continue;          /* not a divisor */
        seq_t q = rev / p;
        if (q <= 1) continue;                 /* need factors >1 */
        if (is_palindrome(p) && is_palindrome(q))
            return true;
    }
    return false;
}

/* Main sequence routine --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 1, for safety */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_m = 1;      /* next integer to test */

    /* Grow cache if needed. Must be able to hold at least n items. */
    while (cache_len < (size_t)n) {
        seq_t m = (seq_t)next_m;
        seq_t cube = m * m * m;                  /* m^3 */
        seq_t rev = reverse_digits(cube);         /* reverse decimal digits */

        if (rev > 1 && passes_palproduct(rev)) {  /* both factors >1 */
            if (cache_len == cache_cap) {        /* enlarge capacity */
                cache_cap = cache_cap ? cache_cap * 2 : 128;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cube;
        }
        ++next_m;
    }
    return cache[(size_t)n - 1];
}
