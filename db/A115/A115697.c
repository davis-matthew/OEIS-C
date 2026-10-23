/*
 * A115697  Powerful(1) numbers (A001694) whose digit reversal is the product
 *          of 2 palindromes greater than 1.
 * https://oeis.org/A115697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   floor_sqrt, is_palindrome, rev_decimal, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) consists of powerful numbers (each prime
   exponent ≥ 2) whose digit reversal is the product of two
   palindromes (greater than 1).  This implementation finds the
   nth such number by brute‑force testing of successive integers.
   It memoises the results so that calls with arbitrary ordering
   remain efficient. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/rev_decimal.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_palindrome.h"
#include "../utils/seq_realloc.h"

/* Return true if n is a powerful number, i.e. every prime exponent
   in the factorisation of n is at least 2. */
static bool is_powerful(seq_t n)
{
    if (n <= 1)
        return false;

    seq_t temp = n;
    seq_t p = 2;
    /* trial division up to sqrt(temp) */
    while (p * p <= temp) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp == 1)
                return false;        /* exponent exactly 1 */
        }
        p += 1;
    }
    /* a remaining factor greater than 1 has exponent 1 */
    if (temp > 1)
        return false;
    return true;
}

/* Return true if the decimal reversal of m can be factored into
   two palindromes (each > 1). */
static bool rev_palindrome_product(seq_t m)
{
    seq_t rev = rev_decimal(m);
    seq_t lim = floor_sqrt(rev);
    for (seq_t p = 2; p <= lim; ++p) {
        if (rev % p != 0)
            continue;
        seq_t q = rev / p;
        if (p > 1 && q > 1 && is_palindrome(p, 10) && is_palindrome(q, 10))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *values   = NULL;
    static size_t len      = 0;
    static size_t cap      = 0;
    static seq_t last_checked = 1;   /* next integer to test */

    while (len < (size_t)n) {
        seq_t candidate = last_checked;
        if (is_powerful(candidate) && rev_palindrome_product(candidate)) {
            /* grow storage if needed */
            if (len == cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = seq_realloc(values, cap * sizeof(seq_t),
                                         newcap * sizeof(seq_t));
                if (!tmp)
                    abort();               /* allocation failure */
                values = tmp;
                cap    = newcap;
            }
            values[len++] = candidate;
        }
        ++last_checked;
    }

    return values[n - 1];
}
