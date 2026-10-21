/*
 * A074775  Numbers n such that tau(n) < tau(n+1) where tau(x)=A000005(x).
 * https://oeis.org/A074775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A074775: Numbers n such that τ(n) < τ(n+1).  
   The function generates terms lazily: each time it is called it
   continues from the last integer examined, counting divisors of
   consecutive integers by a simple sqrt‑factorisation.  Found terms
   are stored in a static array so subsequent calls do not recompute
   earlier results.  The algorithm is O(N√N) for the first N terms,
   comfortably below 2 s for N≈10 000.  
*/

#include <stddef.h>
#include <stdlib.h>

typedef unsigned long long u64;

/* divisor count for 64‑bit integers */
static u64 div_count(u64 n) {
    u64 cnt = 0;
    u64 i   = 1;
    for (u64 sq = 1; sq*sq <= n; ++sq) {
        if (n % sq == 0) {
            if (sq * sq == n) cnt += 1;
            else              cnt += 2;
        }
    }
    return cnt;
}

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *terms = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* static pointer to last integer examined */
    static idx_t cur = 0;

    /* initialise cache */
    if (cap == 0) {
        cap   = 16;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend cache until we have at least n terms */
    while (len < (size_t)n) {
        cur++;                       /* next candidate integer */

        /* compute τ(cur) and τ(cur+1) */
        u64 tau_cur  = div_count((u64)cur);
        u64 tau_next = div_count((u64)(cur + 1));

        if (tau_cur < tau_next) {
            /* store the term, expanding the array if necessary */
            if (len == cap) {
                cap *= 2;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = (seq_t)cur;
        }
    }

    return terms[n - 1];
}
