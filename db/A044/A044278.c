/*
 * A044278  Numbers k such that the string 3,0 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that the base‑9 representation of
 *            k contains the consecutive digits 3,0, but the representation of
 *            k-1 does not
 * Range:     u8 n <= 4, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Return true if the base‑9 representation of x contains a “30” pair. */
static bool has30(seq_t x)
{
    if (x == 0)
        return false;

    seq_t prev = x % (seq_t)9;      /* least significant digit    */
    x = x / (seq_t)9;
    while (x > 0)
    {
        seq_t cur = x % (seq_t)9;
        if (prev == (seq_t)0 && cur == (seq_t)3)
            return true;
        prev = cur;
        x = x / (seq_t)9;
    }
    return false;
}

/* Return the n‑th term of the sequence.  The first term (n==1) is 27. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;
    static idx_t next_k = 1;          /* candidate integer to test */

    if (n == 0)          /* offset is 1 – outside the sequence */
        return 0;

    while (len < n)
    {
        seq_t k = (seq_t)next_k;   /* candidate in seq_t range        */
        if (has30(k) && !has30(k - (seq_t)1))
        {
            if (len == cap)
            {
                size_t new_cap = cap == 0 ? 2 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = k;
        }
        next_k++;
    }

    return cache[n - 1];
}
