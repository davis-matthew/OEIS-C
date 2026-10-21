/*
 * A053521  a(n) = floor(A*a(n-1) + B*a(n-2) + C)/p^r, where p^r is the
 *          highest power of p dividing floor(A*a(n-1) + B*a(n-2) + C),
 *          A=1.0001, B=1.0001, C=1, p=2.
 * https://oeis.org/A053521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (10001*a(n-1)+10001*a(n-2)+10000)/10000 ) /
 *            2^v2(floor( (10001*a(n-1)+10001*a(n-2)+10000)/10000 ))
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=1; for n≥3 define */ 
#include <stdlib.h>

seq_t a(idx_t n) {
    /* memoisation */
    static seq_t *seq = NULL;
    static idx_t last = 0;
    static size_t cap   = 0;

    if (n < 1) return (seq_t)0;               /* out‑of‑range guard */

    /* initialisation */
    if (!seq) {
        cap   = 4;                            /* enough for indices 0..3 */
        seq   = (seq_t*)malloc(cap * sizeof(seq_t));
        seq[0] = (seq_t)0;                    /* unused */
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)1;
        last   = 2;
    }

    /* grow storage if necessary */
    while (last < n) {
        if (last + 1 >= cap) {                /* need more space */
            cap *= 2;
            seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
        }

        /* compute the next term */
        unsigned __int128 v = (unsigned __int128)seq[last]      * 10001ULL
                           + (unsigned __int128)seq[last-1]    * 10001ULL
                           + 10000ULL;
        seq_t t = (seq_t)(v / 10000ULL);        /* floor division */

        /* remove all factors of 2 */
        while ((t & (seq_t)1) == 0) {
            t >>= 1;
        }

        seq[++last] = t;
    }
    return seq[n];
}
