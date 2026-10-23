/*
 * A125756  Numbers n whose reverse binary representation has the following
 *          property: let a 0 mean "halving" and a 1 mean "k -> 3k+1". The
 *          number describes an operation k -> f_n(k). If the equation f_n(k)
 *          = k has a positive integer solution, n is a term in the sequence.
 * https://oeis.org/A125756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence of integers n whose reverse binary representation
 *            yields a linear map k -> (A*k+B)/(2^h) that has a positive
 *            integer fixed point. Computed by brute‑force search
 * Range:     u8 n <= 5, u16 n <= 56, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool is_good(seq_t n)
{
    unsigned __int128 A = 1;   /* numerator of the coefficient of k   */
    unsigned __int128 B = 0;   /* numerator of the constant term      */
    unsigned h = 0;           /* exponent of the denominator 2^h    */
    seq_t t = n;

    while (t != 0)
    {
        if (t & (seq_t)1)
        {
            unsigned __int128 add = ((unsigned __int128)1) << h; /* 2^h   */
            B = 3 * B + add;
            A = 3 * A;
        }
        else
        {
            h++;
        }
        t = t >> 1;
    }

    unsigned __int128 D = ((unsigned __int128)1) << h;   /* 2^h */

    if (A >= D)
    {
        unsigned __int128 diff = A - D;
        if (diff == 0)
            return B == 0;   /* any k satisfies the equation            */
        return false;        /* would give a negative fixed point        */
    }
    else
    {
        unsigned __int128 diff = D - A;  /* strictly positive */
        if (B == 0)
            return false;   /* fixed point would be zero                */
        return (B % diff) == 0;
    }
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t checked_max = 0;

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1 */

    while (cache_len < n)
    {
        checked_max++;
        if (is_good((seq_t)checked_max))
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)checked_max;
        }
    }
    return cache[n - 1];
}
