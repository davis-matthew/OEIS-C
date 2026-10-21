/*
 * A090274  Numbers with a nonzero palindromic digital product (contains no
 *          zeros).
 * https://oeis.org/A090274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   digit_product, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A090274: Numbers whose digits are all nonzero and whose
   digit product is a palindrome (in base 10). */
/* The nth term a(n) is the nth positive integer satisfying those
   conditions. The function uses a lazy cache so it works correctly
   for any order of calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/is_palindrome.h"

/* Test whether a positive integer has no zero digit and its digit
   product is a palindrome. */
static bool is_good(idx_t x)
{
    /* digit_product returns 0 if any digit is zero */
    seq_t prod = digit_product((seq_t)x);
    if (prod == 0)
        return false;
    return is_palindrome(prod, 10);
}

seq_t a(idx_t n)
{
    /* Simple lazy cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static idx_t next_num = 1;   /* next integer to test for goodness */

    if (n == 0) /* undefined for offset 0; return 0 */
        return 0;

    /* keep generating until we have n terms */
    while (cache_len < (size_t)n)
    {
        if (is_good(next_num))
        {
            /* grow cache */
            size_t new_len = cache_len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp)
                return 0; /* allocation failure */
            cache = tmp;
            cache[cache_len] = (seq_t)next_num;
            cache_len = new_len;
        }
        next_num++;
    }
    return cache[n - 1];
}
