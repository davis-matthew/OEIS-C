/*
 * A041747  Denominators of continued fraction convergents to sqrt(393).
 * https://oeis.org/A041747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 92874286*a(n-20) - a(n-40) for n>39, * with a(0)=1,
 *            a(1)=1, a(2)=5, a(3)=6, a(4)=17, a(5)=74, a(6)=91, * a(7)=165,
 *            a(8)=256, a(9)=421, a(10)=5308, a(11)=5729, * a(12)=11037,
 *            a(13)=16766, a(14)=27803, a(15)=127978, * a(16)=283759,
 *            a(17)=411737, a(18)=1930707, a(19)=2342444, * a(20)=90943579...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] == a(0) */
static idx_t  cache_len = 0;     /* number of cached terms (>=1 if cache != NULL) */

/* initialise the first 40 terms using the standard convergent recurrence */
static void init_first_40(void)
{
    const idx_t D   = 393;
    const idx_t a0  = 19;                    /* floor(sqrt(393)) */
    idx_t m  = 0;
    idx_t d  = 1;
    idx_t a  = a0;

    seq_t q_prev2 = (seq_t)0;                /* q_{-1} */
    seq_t q_prev  = (seq_t)1;                /* q_0      */
    cache[0] = q_prev;                       /* a(0) */

    for (idx_t k = 1; k < 40; ++k) {
        idx_t new_m = d * a - m;
        idx_t new_d = (D - new_m * new_m) / d;
        idx_t new_a = (a0 + new_m) / new_d;

        m = new_m;  d = new_d;  a = new_a;

        seq_t new_q = (seq_t)a * q_prev + q_prev2; /* q_k = a_k q_{k-1} + q_{k-2} */
        cache[k] = new_q;

        q_prev2 = q_prev;
        q_prev  = new_q;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* initialise cache to hold first 40 terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 40);
        if (!cache) return (seq_t)0;        /* allocation failure → gracefully return 0 */
        init_first_40();
        cache_len = 40;
    }

    /* value already in cache */
    if (n < cache_len)
        return cache[n];

    /* enlarge the cache */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap <= cache_len) return cache[n]; /* should not happen */

    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) return (seq_t)0;          /* allocation failure → gracefully return 0 */
    cache = tmp;
    /* compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i-20] * (seq_t)92874286u - cache[i-40];
        cache[i] = val;
    }
    cache_len = new_cap;
    return cache[n];
}
