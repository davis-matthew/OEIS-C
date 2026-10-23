/*
 * A044275  Numbers n such that string 2,6 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044275: Numbers n such that the base‑9 representation of n contains
   the substring "26" while the representation of n‑1 does not. The
   function scans integers in order, storing found terms in a static
   cache so that random access to a(n) is efficient.  The implementation
   works for all supported seq_t types, including arbitrary‑precision
   big integers. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true iff x (base 9) contains the substring 26. */
static bool has26(seq_t x)
{
    if (x == 0)
        return false;

    seq_t prev = 9;   /* digit value that cannot occur (base 9 digits are 0–8) */

    while (x > 0)
    {
        seq_t cur = x % 9;           /* remainder in 0..8 */
        if (cur == 2 && prev == 6)
            return true;

        prev = cur;
        x = x / 9;
    }
    return false;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, harmless */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of filled entries */
    static idx_t cache_cap = 0;    /* allocated size */
    static seq_t last_checked = 0; /* last integer that was tested */

    if (n <= cache_size)
        return cache[n - 1];   /* already computed */

    for (seq_t cand = last_checked + 1; cache_size < n; ++cand)
    {
        if (has26(cand) && !has26(cand - 1))
        {
            if (cache_size == cache_cap)   /* grow cache if needed */
            {
                idx_t new_cap = (cache_cap == 0 ? 16 : cache_cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)  /* allocation failed – abort */
                    exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        last_checked = cand;
    }
    return cache[n - 1];
}
