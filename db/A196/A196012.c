/*
 * A196012  Number of n X 3 0..4 arrays with each element equal to the number
 *          its horizontal and vertical neighbors within one of itself.
 * https://oeis.org/A196012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-4) - a(n-5) - 2*a(n-6) Initial
 *            values: a(1)=1, a(2)=1, a(3)=2, a(4)=7, a(5)=13, a(6)=16. This
 *            implementation stores computed terms in a static cache to handle
 *            arbitrary call order. The routine works for any integer type
 *            seq_t and index type idx_t supplied by the bui...
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of previously computed values: cache[0] holds a(1) */
static seq_t *cache = NULL;      /* pointer to cache array               */
static size_t   cap   = 0;       /* current allocated capacity            */
static idx_t    computed = 0;    /* highest index currently stored        */

/* initialise cache with the first six terms */
static void init_cache(void)
{
    if (computed != 0) return;           /* already initialised                 */
    size_t init_cap = 16;
    cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
    if (!cache) exit(1);                 /* allocation failure                  */
    cache[0] = (seq_t)1;                 /* a(1) */
    cache[1] = (seq_t)1;                 /* a(2) */
    cache[2] = (seq_t)2;                 /* a(3) */
    cache[3] = (seq_t)7;                 /* a(4) */
    cache[4] = (seq_t)13;                /* a(5) */
    cache[5] = (seq_t)16;                /* a(6) */
    computed = 6;                        /* we now have terms up to a(6)        */
    cap = init_cap;                      /* set allocated capacity              */
}

/* extend cache to hold at least 'need' elements */
static void grow_cache(idx_t need)
{
    if (cap >= (size_t)need) return;
    size_t new_cap = cap ? cap : 16;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                   /* allocation failure                  */
    cache = tmp;
    cap = new_cap;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1 but harmless */
    init_cache();                         /* ensure cache contains first 6 terms */

    if (n <= computed)                     /* already stored */
        return cache[n-1];

    grow_cache(n);                         /* make space for up to n             */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val;
        if (i >= 7) {                    /* recurrence for i >= 7             */
            val = cache[i-2] + cache[i-3];   /* a(i-1)+a(i-2)                    */
            val += 3 * cache[i-5];           /* +3*a(i-4)                         */
            val -= cache[i-6];               /* -a(i-5)                           */
            val -= 2 * cache[i-7];           /* -2*a(i-6)                         */
        } else {                         /* should not occur, but safe         */
            switch ((int)i) {
                case 1: val = (seq_t)1; break;
                case 2: val = (seq_t)1; break;
                case 3: val = (seq_t)2; break;
                case 4: val = (seq_t)7; break;
                case 5: val = (seq_t)13; break;
                case 6: val = (seq_t)16; break;
                default: val = (seq_t)0; break;
            }
        }
        cache[i-1] = val;                 /* store newly computed term          */
    }
    computed = n;                         /* update count of stored terms        */
    return cache[n-1];
}
