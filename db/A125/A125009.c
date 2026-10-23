/*
 * A125009  Numbers k for which nontrivial positive magic squares of exactly 2
 *          different orders with magic sum k exist. For a definition of
 *          nontrivial positive magic squares, see A125005.
 * https://oeis.org/A125009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: sequence of all k such that exactly two different orders
   n (n ≥ 3) give non‑trivial positive magic squares with magic sum k.
   For each k we count the orders satisfying the rules:
     • if n is odd:   k ≡ 0   (mod n)
     • if n is even:  k ≡ n/2 (mod n)
   A k is included when the count equals 2.  The function keeps a
   cache of found terms.  It is ready for both 64‑bit and
   arbitrary‑precision seq_t.  The code compiles as C11 and C++17.   */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache of found terms */
    static seq_t *terms = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Next candidate k to test */
    static seq_t next_k = 1;

    /* Generate terms until we have at least n of them */
    while (len < n)
    {
        seq_t k = next_k++;
        size_t cnt = 0;

        /* Check orders starting from 3, stop when the minimal
           magic sum for an order exceeds k. */
        for (idx_t order = 3; ; ++order)
        {
            /* N(order) = order * (order^2 + 1) / 2 */
            seq_t order_sq = (seq_t)order * (seq_t)order;
            seq_t N = (seq_t)order * (order_sq + 1) / 2;

            if (N > k)
                break;

            if ((order & 1) == 0)            /* even order */
            {
                if ((k % (seq_t)order) == (order >> 1))
                    ++cnt;
            }
            else                             /* odd order */
            {
                if ((k % (seq_t)order) == 0)
                    ++cnt;
            }

            if (cnt > 2)                     /* no need to count further */
                break;
        }

        if (cnt == 2)                          /* store k */
        {
            if (len + 1 > cap)
            {
                size_t new_cap = cap ? cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)                     /* allocation failure */
                    return 0;
                terms = tmp;
                cap   = new_cap;
            }
            terms[len++] = k;
        }
    }

    return terms[n - 1];
}
