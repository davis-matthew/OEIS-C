/*
 * A039466  Numbers whose base-11 representation has the same nonzero number
 *          of 3's and 5's.
 * https://oeis.org/A039466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑11 representation has the same nonzero number of
   digits 3 and 5.  The function searches for the terms on demand. */

#include <stdbool.h>
#include <stddef.h>

/* Static buffer to store found terms.  200 000 entries are more
   than enough for n ≤ 10 000; the buffer is large enough that the
   function never has to grow it.  The size is a compile‑time
   constant, so the code compiles in both C and C++.  */
static seq_t cache[200000];
static size_t cache_len = 0;   /* how many terms are filled */
static seq_t cur = 0;          /* current candidate being tested */

/* Return true if x has the same number of 3’s and 5’s (both non‑zero)
   in its base‑11 representation. */
static inline bool valid(seq_t x)
{
    int c3 = 0, c5 = 0;
    seq_t y = x;
    while (y > 0)
    {
        seq_t d = y % 11;
        if (d == 3) c3++;
        else if (d == 5) c5++;
        y /= 11;
    }
    return (c3 == c5) && (c3 > 0);
}

/* OEIS sequence accessor.  n ≥ 1.  The function is memoised: once a
   term is found it is stored in the static cache so that subsequent
   calls are O(1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1 – guard against misuse */

    size_t target = (size_t)n;
    while (cache_len < target)
    {
        cur++;                         /* next integer candidate */
        if (valid(cur))
            cache[cache_len++] = cur;
    }
    return cache[n - 1];
}
