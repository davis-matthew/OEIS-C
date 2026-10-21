/*
 * A072016  Numbers k such that gcd(k, reverse(k)) = 27 = 3^3, where
 *          reverse(x) = A004086(x).
 * https://oeis.org/A072016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 180, u32 n <= 1030, u64 n <= 1030,
 *            u128 n <= 1030, big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072016: numbers k such that gcd(k, reverse(k)) = 27
   (reverse is decimal reverse A004086).  The function builds the
   terms on demand and caches them for subsequent calls.  No external
   utilities are required. */
#include <stdlib.h>
#include "../utils/gcd_seq.h"

static seq_t reverse_seq(seq_t n)
{
    seq_t rev = 0;
    while (n > 0)
    {
        rev = rev * 10 + (n % 10);
        n = n / 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    /* static storage for computed terms */
    static seq_t *terms   = NULL;   /* array of terms */
    static size_t  cap    = 0;     /* allocated capacity */
    static size_t  used   = 0;     /* number of terms stored */
    static seq_t   next   = (seq_t)27; /* next candidate to test */

    if (n == 0)
        return 0;                /* undefined, but avoid crash */

    /* grow array while we need more terms */
    while (used < (size_t)n)
    {
        /* test candidate */
        seq_t  k   = next;
        next++;

        /* quick divisibility by 27 */
        if (k % (seq_t)27 != 0)
            continue;

        seq_t revk = reverse_seq(k);
        if (revk % (seq_t)27 != 0)
            continue;

        /* compute and test the gcd */
        seq_t g = gcd_seq(k, revk);
        if (g != (seq_t)27)
            continue;   /* not a term */

        /* store term */
        if (used >= cap)
        {
            size_t newcap = cap == 0 ? 32 : cap * 2;
            seq_t *tmp = (seq_t*)realloc(terms, newcap * sizeof(seq_t));
            if (!tmp) /* allocation failure */
                exit(1);
            terms = tmp;
            cap   = newcap;
        }
        terms[used++] = k;
    }

    return terms[n - 1];
}
