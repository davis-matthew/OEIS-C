/*
 * A187502  Let i be in {1,2,3,4} and let r >= 0 be an integer. Let p = {p_1,
 *          p_2, p_3, p_4} = {-2,0,1,2}, n=3*r+p_i, and define a(-2)=0. Then
 *          a(n)=a(3*r+p_i) gives the quantity of H_(9,4,0) tiles in a
 *          subdivided H_(9,i,r) tile after linear scaling by the factor Q^r,
 *          where Q=sqrt(x^2-1) with x=2*cos(Pi/9).
 * https://oeis.org/A187502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-3) - 3*a(n-9) for n >= 12 a(0) = 0, a(1) = 0, a(2)
 *            = 1, a(3) = 1, a(4) = 1, a(5) = 1, a(6) = 2, a(7) = 2, a(8) = 3,
 *            a(9) = 5, a(10) = 6, a(11) = 7
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static size_t seq_cache_size = 0;
static size_t seq_cache_cap  = 0;

static inline void ensure_seq_cache_cap(size_t new_cap) {
    if (new_cap <= seq_cache_cap) return;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (seq_cache) {
        for (size_t i = 0; i < seq_cache_size; ++i) new_cache[i] = seq_cache[i];
        free(seq_cache);
    }
    seq_cache = new_cache;
    seq_cache_cap = new_cap;
}

seq_t a(idx_t n) {
    /* Check if result already cached */
    if ((size_t)n < seq_cache_size) return seq_cache[n];

    /* Prepare initial values array (size 12) */
    static const seq_t init_vals[12] = {
        (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)2, (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)6, (seq_t)7
    };

    /* Grow cache to accommodate index n */
    size_t target = (size_t)n + 1;
    ensure_seq_cache_cap(target);

    /* Compute new terms */
    for (size_t i = seq_cache_size; i < target; ++i) {
        if (i < 12) {
            seq_cache[i] = init_vals[i];
        } else {
            /* a(i) = 3*a(i-3) - 3*a(i-9) */
            seq_t t1 = seq_cache[i - 3];
            seq_t t2 = seq_cache[i - 9];
            seq_cache[i] = (seq_t)3 * t1 - (seq_t)3 * t2;
        }
    }
    seq_cache_size = target;

    return seq_cache[n];
}
