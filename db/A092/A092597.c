/*
 * A092597  Natural numbers n for which sum of decimal digits is not smaller
 *          than n/3.
 * https://oeis.org/A092597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092597: numbers n such that the sum of decimal digits of n
   satisfies sum_digits(n) > n/3.  a(1)=1, a(2)=2, …  */

#include <stddef.h>

/* Static cache to hold all terms of the finite sequence.  The sequence
   has only 17 terms, but the array is sized generously. */
static seq_t cache[100];
static size_t cache_len = 0;
static int cache_ready = 0;

/* Build the cache once.  We scan integers starting at 1 until the
   current integer n exceeds the bound 27 * digits(n); after that
   the inequality sum_digits(n) > n/3 can never hold. */
static void build_cache(void)
{
    if (cache_ready)
        return;

    size_t len = 0;
    idx_t n = 1;

    while (1) {
        /* compute digit sum of n */
        seq_t sum = 0;
        idx_t t = n;
        while (t > 0) {
            sum += (seq_t)(t % 10);
            t /= 10;
        }

        /* strict inequality: sum > n/3  <=>  sum*3 > n */
        if (sum * 3 > (seq_t)n)
            cache[len++] = (seq_t)n;

        /* number of decimal digits of n */
        idx_t digits = 0;
        t = n;
        while (t > 0) {
            digits++;
            t /= 10;
        }

        /* if n > 27*digits, no larger number can satisfy the property */
        if (n > 27 * digits)
            break;

        n++;  /* next candidate */
    }

    cache_len = len;
    cache_ready = 1;
}

/* Return the nth term of the sequence (1-indexed).  For indices larger
   than the number of existing terms the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence is 1-indexed */

    build_cache();

    if ((size_t)n <= cache_len)
        return cache[n - 1];
    else
        return 0;
}
