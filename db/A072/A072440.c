/*
 * A072440  3-smooth numbers k with decimal digits <= 3.
 * https://oeis.org/A072440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   digits_leq_3, ensure_vector
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search over 3‑smooth numbers (2^i * 3^j),
   keeping only those whose decimal digits are ≤ 3.  The sequence is
   generated in increasing order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits_leq_3.h"
#include "../utils/ensure_vector.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined, for safety */

    /* static state for memoization */
    static seq_t *humble = NULL;      /* all 3‑smooth numbers so far          */
    static size_t hlen = 0, hcap = 0;
    static idx_t i2 = 0, i3 = 0;      /* indices for next factor 2 and 3      */

    static seq_t *cache = NULL;       /* filtered numbers (the requested seq)*/
    static size_t clen = 0, ccap = 0;

    /* allocate cache if not yet done */
    if (cache == NULL)
    {
        ccap = 4;
        cache = (seq_t *)malloc(ccap * sizeof(seq_t));
        clen = 0;
    }

    /* allocate humble array if not yet done */
    if (humble == NULL)
    {
        hcap = 4;
        humble = (seq_t *)malloc(hcap * sizeof(seq_t));
        hlen = 0;
    }

    /* initialize the very first humble number (1) */
    if (hlen == 0)
    {
        ensure_vector(&humble, &hcap, 1);
        humble[0] = 1;
        hlen = 1;
        /* 1 itself passes the digit test */
        ensure_vector(&cache, &ccap, clen + 1);
        cache[clen++] = 1;
    }

    /* generate more until we've collected the n‑th filtered element */
    while (clen < n)
    {
        seq_t nxt2 = humble[i2] * 2;
        seq_t nxt3 = humble[i3] * 3;
        seq_t next = (nxt2 < nxt3) ? nxt2 : nxt3;

        if (next != humble[hlen - 1])            /* avoid duplicates      */
        {
            /* add to humble list */
            ensure_vector(&humble, &hcap, hlen + 1);
            humble[hlen++] = next;

            /* keep it if it only uses digits 0–3 */
            if (digits_leq_3(next))
            {
                ensure_vector(&cache, &ccap, clen + 1);
                cache[clen++] = next;
            }
        }

        /* advance pointers for which factor produced the current minimum */
        if (next == nxt2) ++i2;
        if (next == nxt3) ++i3;
    }

    return cache[n - 1];
}
