/*
 * A144830  Numerators of the convergents to x = 1/(x^4+1).
 * https://oeis.org/A144830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numerators of the convergents to x = 1/(x^4+1)  */
/*  Define sequences p_n (numerators) and q_n (denominators):         */
/*    p_0 = 1, q_0 = 1                                            */
/*    p_{n+1} = q_n^4                                               */
/*    q_{n+1} = p_n^4 + q_n^4                                       */
/*  The required term a(n) is simply p_n.                          */

#include <stdlib.h>

static seq_t *num_cache = NULL;   /* stores p_n      */
static seq_t *den_cache = NULL;   /* stores q_n      */
static idx_t cache_cap = 0;       /* capacity of the arrays   */
static idx_t cache_len = 0;       /* number of computed terms */

/* Ensure the cache can hold indices up to n inclusive. */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *new_num = (seq_t *)realloc(num_cache, new_cap * sizeof(seq_t));
    if (!new_num) exit(1);
    seq_t *new_den = (seq_t *)realloc(den_cache, new_cap * sizeof(seq_t));
    if (!new_den) exit(1);

    num_cache = new_num;
    den_cache = new_den;
    cache_cap = new_cap;
}

/* Compute and return the nth term a(n) */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache_len == 0) {
        ensure_cache(0);
        num_cache[0] = 1;
        den_cache[0] = 1;
        cache_len   = 1;
    }

    /* Grow cache up to n */
    if (cache_len <= n) {
        ensure_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev_p = num_cache[i-1];
            seq_t prev_q = den_cache[i-1];

            seq_t prev_q2 = prev_q * prev_q;
            seq_t prev_q4 = prev_q2 * prev_q2;

            seq_t prev_p2 = prev_p * prev_p;
            seq_t prev_p4 = prev_p2 * prev_p2;

            num_cache[i] = prev_q4;                     /* p_i = q_{i-1}^4 */
            den_cache[i] = prev_p4 + prev_q4;           /* q_i = p_{i-1}^4 + q_{i-1}^4 */
        }
        cache_len = n + 1;
    }

    return num_cache[n];
}
