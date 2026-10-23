/*
 * A166135  Number of possible paths to each node that lies along the edge of
 *          a cut 4-nomial tree, that is rooted one unit from the cut.
 * https://oeis.org/A166135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A166135: Number of possible paths to each node on the edge of a cut 4‑nomial tree.
 *
 * This implementation uses the proven linear recurrence (valid for n ≥ 5):
 *
 *   a(n) = ( |C(n)|*a(n‑2) + |D(n)|*a(n‑3) – B(n)*a(n‑1) ) / ( 2*n*(2*n + 1) )
 *
 * where
 *
 *   B(n) =  17*n² – 53*n + 24
 *   C(n) =  6*( –13*n² + 43*n – 36 )
 *   D(n) = –108*( 2*n – 5 )*( n – 3 )
 *
 * Initial values (offset 1):
 *
 *   a(1) = 1,   a(2) = 1,   a(3) = 3,   a(4) = 7
 *
 * The function keeps a static memoised table so that repeated calls are O(1) after the
 * first evaluation up to the requested index.  For 64‑bit builds 128‑bit temporaries are
 * used to avoid overflow in intermediate products; for arbitrary‑precision builds the
 * built‑in operations of the ``seq_t`` type are used directly.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static data for memoisation */
    static seq_t *cache      = NULL;   /* table: cache[i] holds a(i) for i >= 1 */
    static idx_t  capacity   = 0;      /* number of allocated slots (index 0 unused) */
    static idx_t  max_calc   = 0;      /* highest index already computed */

    if (n == 0) return 0;              /* value outside the required offset */

    /* initialise cache on first use */
    if (capacity == 0) {
        capacity = 5;                  /* at least enough for indices 1..4 */
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!cache) return 0;          /* fault tolerance – return 0 on OOM */

        /* base values */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 3;
        cache[4] = 7;
        max_calc = 4;
    }

    /* grow cache if needed */
    if (n + 1 > capacity) {
        idx_t new_cap = capacity * 2;
        if (new_cap < capacity) new_cap = capacity + 1;  /* overflow protection */
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0; /* OOM – return 0 */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = max_calc + 1; i <= n; ++i) {
        if (i <= 4) {
            /* base values already in place */
            continue;
        }

        /* cast index to signed 64‑bit for coefficient arithmetic */
        int64_t nn = (int64_t)i;

        /* coefficients (signed 64‑bit) */
        int64_t B  = 17 * nn * nn - 53 * nn + 24;
        int64_t C  = 6 * (-13 * nn * nn + 43 * nn - 36);
        int64_t D  = -108 * (2 * nn - 5) * (nn - 3);
        int64_t den= 2 * nn * (2 * nn + 1);

        /* absolute values of the possibly negative C and D */
        int64_t absC = C >= 0 ? C : -C;
        int64_t absD = D >= 0 ? D : -D;

        /* compute numerator and result depending on seq_t size */
        if (sizeof(seq_t) <= 8) {  /* 64‑bit build – use 128‑bit temporaries */
            __int128 termB = (__int128)cache[i-1] * (__int128)B;
            __int128 termC = (__int128)cache[i-2] * (__int128)absC;
            __int128 termD = (__int128)cache[i-3] * (__int128)absD;

            __int128 numerator = termC + termD - termB;
            cache[i] = (seq_t)(numerator / (__int128)den);
        } else {   /* arbitrary‑precision build – rely on operator overloading */
            seq_t termB = cache[i-1] * (seq_t)B;
            seq_t termC = cache[i-2] * (seq_t)absC;
            seq_t termD = cache[i-3] * (seq_t)absD;

            seq_t numerator = termC + termD - termB;
            cache[i] = numerator / (seq_t)den;
        }
    }

    max_calc = (n > max_calc) ? n : max_calc;
    return cache[n];
}
