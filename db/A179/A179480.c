/*
 * A179480  Let m>k>0 be odd numbers and denote by the symbol "m<->k" the
 *          value A000265(m-k). Then the sequence m<->k, m<->(m<->k),
 *          m<->(m<->(m<->k)), ... is periodic; a(n) is the smallest period in
 *          the case m=2*n-1, k=1.
 * https://oeis.org/A179480
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   odd_half_odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A179480.
 *
 *  For a given n>=2 let m = 2*n-1 (an odd number) and k=1.
 *  Define f(x) = oddPart( (m - x) / 2 ), i.e. divide (m-x) by 2
 *  and repeatedly divide by 2 until the result is odd.
 *  Start the sequence with v0 = f(m-k).  Then each step vi+1 = f(vi)
 *  The sequence is eventually periodic; a(n) is the length of that
 *  (minimal) period.  This routine returns that period.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/odd_half_odd_part.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined for the offset */

    idx_t m = 2 * n - 1;          /* odd m */

    /* start value v0 = f(m-1) */
    idx_t cur = odd_half_odd_part(m - 1);

    /* static visited array reused across calls */
    static idx_t *vis = NULL;
    static idx_t vis_size = 0;

    if (m + 1 > vis_size) {
        /* resize array; use size_t to avoid overflow for realloc size */
        size_t new_size = (size_t)(m + 1);
        idx_t *tmp = (idx_t *)realloc(vis, new_size * sizeof(idx_t));
        if (!tmp) abort();
        vis = tmp;
        vis_size = (idx_t)new_size;
    }
    /* initialise with 0 (meaning "not seen") */
    memset(vis, 0, vis_size * sizeof(idx_t));

    idx_t step = 0;
    vis[cur] = step + 1;          /* record first visit */

    for (;;) {
        idx_t diff = m - cur;          /* m - current value */
        idx_t nxt = odd_half_odd_part(diff); /* next value */
        if (vis[nxt] != 0) {
            /* cycle found: period = current step - first step of nxt */
            idx_t period = step + 1 - (vis[nxt] - 1);
            return (seq_t)period;
        }
        step++;
        vis[nxt] = step + 1;
        cur = nxt;
    }
}
