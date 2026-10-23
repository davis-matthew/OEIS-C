/*
 * A119466  Consider the standard game of Nim with 3 heaps and make a list of
 *          the losing positions (x,y,z) with x <= y <= z sorted by sum, ties
 *          broken by putting smallest value of x first, then y, then z;
 *          sequence gives z values.
 * https://oeis.org/A119466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = z of the n-th Nim‑losing triple (x,y,z) with * x <= y <=
 *            z, where x ^ y ^ z == 0, sorted by sum (x+y+z), then *
 *            lexicographically by x, y, z. The first triple is (0,0,0) and *
 *            a(1)=0
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the generated z‑values */
static seq_t *zlist = NULL;
static size_t  zlen  = 0;    /* number of values stored */
static size_t  zcap  = 0;    /* current capacity */
static idx_t   nexts = 0;    /* sum of the next triple to generate */

/* Grow the array when capacity is exceeded. */
static void grow_zlist(void)
{
    size_t newcap = zcap == 0 ? 64 : zcap << 1;
    seq_t *tmp = (seq_t *)realloc(zlist, newcap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    zlist = tmp;
    zcap  = newcap;
}

/* Return the n-th term of sequence A119466 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, but harmless */

    /* Generate triples until we have at least n z‑values.  No early
       termination inside a sum; we exhaust the whole current sum so
       that subsequent higher n calls still receive the missing values. */
    while (zlen < n) {
        idx_t s = nexts;                    /* current sum */

        for (idx_t x = 0; x <= s / 3; ++x) {
            idx_t r      = s - x;
            idx_t max_y = r / 2;              /* ensures y <= z */

            for (idx_t y = x; y <= max_y; ++y) {
                idx_t z = r - y;              /* z >= y automatically */

                if (((seq_t)x ^ (seq_t)y ^ (seq_t)z) == (seq_t)0) {
                    if (zlen >= zcap) grow_zlist();
                    zlist[zlen++] = (seq_t)z;
                }
            }
        }

        ++nexts;        /* proceed to the next sum after exhausting current one */
    }

    return zlist[n - 1];
}
