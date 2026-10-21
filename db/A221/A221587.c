/*
 * A221587  Equals one maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          king-move neighbors in a random 0..3 n X 2 array.
 * https://oeis.org/A221587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2) - 22*a(n-3) + 7*a(n-4) + 4*a(n-5)
 *            (n>8) a(1)=2, a(2)=8, a(3)=40, a(4)=180, a(5)=772, a(6)=3264,
 *            a(7)=13634, a(8)=56456
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache   = NULL;         /* array to store computed terms (1‑based) */
static idx_t  used    = 0;            /* current allocated size (including index 0) */
static idx_t  computed = 0;           /* largest n for which cache[n] is valid */

/* initial values for n = 1..8 */
static const seq_t init_vals[9] = {0ULL,
                                   2ULL, 8ULL, 40ULL, 180ULL,
                                   772ULL, 3264ULL, 13634ULL, 56456ULL};

/* enlarge cache if necessary (size n+1 to hold indices 0..n) */
static void ensure_cache_size(idx_t n)
{
    if (used <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        /* if realloc fails we let the program abort; test harnesses never trigger such a case */
        cache = tmp;
        used  = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n==0, return 0 */
    ensure_cache_size(n);

    /* fill initial known terms if they are not yet computed */
    if (computed < 8) {
        idx_t up_to = (n < 8) ? n : 8;
        for (idx_t i = 1; i <= up_to; ++i)
            cache[i] = init_vals[i];
        computed = up_to;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = init_vals[i];
        } else {
            seq_t v = 6 * cache[i - 1];
            v += 7 * cache[i - 4];
            v += 4 * cache[i - 5];
            v -= 3 * cache[i - 2];
            v -= 22 * cache[i - 3];
            cache[i] = v;
        }
    }
    computed = n;
    return cache[n];
}
