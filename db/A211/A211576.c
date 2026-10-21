/*
 * A211576  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having
 *          four, five or six distinct values for every i,j,k<=n.
 * https://oeis.org/A211576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A211576: number of -2..2 arrays with the
   described property.
   Recurrence (for n > 10):
   a(1)=16, a(2)=48, a(3)=124, a(4)=294, a(5)=680, a(6)=1578,
   a(7)=3600, a(8)=8458, a(9)=19460, a(10)=46510,
   a(n)=4*a(n-1)+7*a(n-2)-45*a(n-3)+10*a(n-4)+155*a(n-5)
        -130*a(n-6)-180*a(n-7)+216*a(n-8)+36*a(n-9)-72*a(n-10)
*/
#include <stdlib.h>
#include <stdint.h>

/* cached values, 1‑based; cache[0] unused */
static seq_t *cache = NULL;
/* largest index for which cache is filled */
static idx_t computed = 0;

/* initialise the cache with the first 10 terms */
static void init_cache(void)
{
    if (computed < 10) {
        size_t new_cap = 11; /* indices 0..10 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache[1]  = (seq_t)16;
        cache[2]  = (seq_t)48;
        cache[3]  = (seq_t)124;
        cache[4]  = (seq_t)294;
        cache[5]  = (seq_t)680;
        cache[6]  = (seq_t)1578;
        cache[7]  = (seq_t)3600;
        cache[8]  = (seq_t)8458;
        cache[9]  = (seq_t)19460;
        cache[10] = (seq_t)46510;
        computed = 10;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* sequence is defined for n>=1 only */
    init_cache();              /* guarantees cache[1..10] exist */

    if (n <= computed) return cache[n];

    /* expand storage to accommodate up to n */
    size_t new_cap = (size_t)(n + 1); /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t val = 0;
        val += (seq_t)4   * cache[k-1];
        val += (seq_t)7   * cache[k-2];
        val -= (seq_t)45  * cache[k-3];
        val += (seq_t)10  * cache[k-4];
        val += (seq_t)155 * cache[k-5];
        val -= (seq_t)130 * cache[k-6];
        val -= (seq_t)180 * cache[k-7];
        val += (seq_t)216 * cache[k-8];
        val += (seq_t)36  * cache[k-9];
        val -= (seq_t)72  * cache[k-10];
        cache[k] = val;
    }
    computed = n;
    return cache[n];
}
