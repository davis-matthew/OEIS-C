/*
 * A044387  Numbers n such that string 5,5 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044387: numbers n such that "55" occurs in n but not in n-1
   Computed by scanning integers and selecting numbers where has_55(n) && !has_55(n-1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Check whether the base‑10 representation of x contains the adjacent
   digits 5,5. */
static bool has55(idx_t x)
{
    if (x < 55)
        return false;
    idx_t prev = x % 10;
    x /= 10;
    while (x)
    {
        idx_t d = x % 10;
        if (d == 5 && prev == 5)
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static storage for the sequence */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* current allocation size */
    static size_t len = 0;      /* number of terms stored */
    static idx_t last_scan = 0; /* last integer examined */
    static bool prev_has_55 = false; /* has_55(last_scan) */

    if (n == 0)
        return (seq_t)0; /* undefined for offset 1, but harmless */

    while (len < n)
    {
        idx_t current = ++last_scan;
        bool cur_has = has55(current);

        if (cur_has && !prev_has_55)
        {
            /* grow storage if necessary */
            if (len == cap)
            {
                cap = (cap == 0) ? 4 : (cap << 1);
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[len++] = (seq_t)current;
        }
        prev_has_55 = cur_has;
    }

    return cache[n - 1];
}
