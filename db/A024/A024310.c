/*
 * A024310  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n+1-k), where k =
 *          floor((n+1)/2), s = (natural numbers >= 2), t = (Lucas numbers).
 * https://oeis.org/A024310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=2..⌊(n+3)/2⌋} j * Lucas(n-j+2) Lucas numbers
 *            satisfy L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1u)
        return 0;                     /* N < 1 is out of range --------- */

    /* m = ⌊(n+3)/2⌋ */
    idx_t m = (n + 3u) >> 1u;

    /* ---------------------------------------------- */
    /*  Compute Lucas numbers up to index n+2.  The array
        is kept static so that it grows only when a larger
        n is requested.  This memoises the sequence over
        all calls. -------------------------------- */
    static seq_t *luc = NULL;          /* array of Lucas numbers */
    static idx_t luc_size = 0;         /* number of elements currently stored */

    if (luc_size <= n + 2u) {          /* need indices 0 .. n+2 */
        idx_t new_size = n + 3u;       /* allocate one extra for safety   */
        seq_t *tmp = (seq_t *)realloc(luc, new_size * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);       /* allocation failed ------------- */
        luc = tmp;

        if (luc_size == 0u) {          /* initialise base values */
            luc[0] = (seq_t)2;
            luc[1] = (seq_t)1;
            luc_size = 2u;
        }

        /* fill in remaining Lucas numbers */
        for (idx_t i = luc_size; i < new_size; ++i)
            luc[i] = luc[i-1] + luc[i-2];
        luc_size = new_size;
    }

    /* ---------------------------------------------- */
    /*  Sum the required product terms.  The loop runs
        at most ⌊(n+3)/2⌋ times, which is O(n) but the
        total work over all calls is linear in the
        maximum n requested. --------------------- */
    seq_t sum = 0;
    for (idx_t j = 2u; j <= m; ++j)
        sum += (seq_t)j * luc[n - j + 2u];

    return sum;
}
