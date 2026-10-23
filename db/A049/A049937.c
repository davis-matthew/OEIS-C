/*
 * A049937  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 2.
 * https://oeis.org/A049937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)+...+a(n-1)+a(2^ceil(log2(n-1))+2-n) Initial values:
 *            a(1)=1, a(2)=1, a(3)=2 (OEIS A049937)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of computed a(i) and their prefix sums */
static seq_t *a_cache = NULL;
static seq_t *s_cache = NULL;
static size_t cache_cap = 0;      /* number of entries allocated in the arrays */
static idx_t  cache_size = 0;     /* highest n for which a(n) is known */

/* small helper: next power of two >= v (v>0) */
static idx_t next_power_of_two(idx_t v)
{
    idx_t p = 1;
    while (p < v) p <<= 1;
    return p;
}

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* enlarge cache if necessary: need capacity > n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (cache_cap > 0 ? cache_cap : 16);
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *new_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        seq_t *new_s = (seq_t *)realloc(s_cache, new_cap * sizeof(seq_t));
        if (!new_a || !new_s) { exit(1); }

        a_cache = new_a;
        s_cache = new_s;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > cache_size) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {
                a_cache[i] = (seq_t)1;
                s_cache[i] = (seq_t)1;
            } else if (i == 2) {
                a_cache[i] = (seq_t)1;
                s_cache[i] = (seq_t)2;          /* 1+1 */
            } else if (i == 3) {
                a_cache[i] = (seq_t)2;
                s_cache[i] = (seq_t)4;          /* 1+1+2 */
            } else {
                idx_t v   = i - 1;
                idx_t pow = next_power_of_two(v);
                idx_t m   = pow + 2 - i;        /* guaranteed 1 <= m < i */

                seq_t a_m = a_cache[m];
                seq_t sum_prev = s_cache[i - 1];
                seq_t a_i = sum_prev + a_m;

                a_cache[i] = a_i;
                s_cache[i] = sum_prev + a_i;    /* prefix sum up to i */
            }
        }
        cache_size = n;
    }

    return a_cache[n];
}
