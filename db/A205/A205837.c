/*
 * A205837  Numbers k for which 2 divides s(k)-s(j) for some j<k; each k
 *          occurs once for each such j; s(k) denotes the (k+1)-st Fibonacci
 *          number.
 * https://oeis.org/A205837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes OEIS A205837: Numbers k for which 2 divides s(k)-s(j) for some j<k;
 * each k occurs once for each such j; s(k) is the (k+1)-st Fibonacci number.
 * The algorithm counts, for each k, how many j<k give an even difference.
 * The parity of the Fibonacci numbers repeats every 3: F_n is even iff n%3==0.
 * For s(k)=F_{k+1}, s(k) is even iff (k+1)%3==0 <=> k%3==2.
 * Thus:
 *   if k%3==2 : we count j with j%3==2   (even indices)
 *   else      : we count j with j%3!=2   (odd indices)
 * The count for each k is added to a cumulative sum; a(n) is the minimal k
 * such that the cumulative sum ≥ n.  The counts are pre‑computed incrementally
 * and memoised so that multiple calls are fast.
 */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static state: cumulative sums of counts up to each k */
    static idx_t *cum = NULL;
    static size_t cum_cap = 0;
    static idx_t max_k = 0;      /* last k for which cum[k] is valid */

    /* initialise on first call */
    if (cum == NULL) {
        cum_cap = 16;
        cum = (idx_t*)malloc(cum_cap * sizeof(idx_t));
        if (!cum) return 0;     /* memory failure, return 0 as a safe fallback */
        cum[0] = 0;             /* cumulative count for k=0 */
        max_k = 0;
    }

    /* extend cumulative array until we reach the requested n */
    while (cum[max_k] < n) {
        idx_t k = max_k + 1;          /* next k to process */
        idx_t t = k - 1;              /* number of candidates j < k */
        idx_t count;

        if (k <= 2) {
            count = 0;                 /* none for k=1,2 */
        } else if (k % 3 == 2) {       /* s(k) even, need j%3==2 */
            if (t >= 2) {
                count = (t - 2) / 3 + 1;
            } else {
                count = 0;
            }
        } else {                       /* s(k) odd, need j%3!=2 */
            idx_t m = (t >= 2) ? ((t - 2) / 3 + 1) : 0;   /* number of j%3==2 */
            count = t - m;
        }

        /* ensure capacity for cum[k] */
        size_t needed = (size_t)(k + 1);
        if (needed > cum_cap) {
            size_t new_cap = cum_cap * 2;
            if (new_cap < needed) new_cap = needed;
            idx_t *new_arr = (idx_t*)realloc(cum, new_cap * sizeof(idx_t));
            if (!new_arr) return 0;     /* memory failure */
            cum = new_arr;
            cum_cap = new_cap;
        }

        cum[k] = cum[max_k] + count;
        max_k = k;
    }

    return (seq_t)max_k;
}
