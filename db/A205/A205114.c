/*
 * A205114  Least k such that n divides L(k)-L(j) for some j satisfying
 *          1<=j<k, where L(j) is the j-th Lucas number (A000032).
 * https://oeis.org/A205114
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

/* Algorithm: find the smallest k such that
   there exists 1 <= j < k with n | L(k)-L(j),
   where L(i) is the i‑th Lucas number.  This
   is equivalent to the first repeated value in
   the Lucas sequence modulo n. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Guard against n == 0 (undefined) */
    if (n == 0) return (seq_t)0;

    /* Array to store first occurrence of each residue 0..n-1 */
    size_t sz = (size_t)n;
    idx_t *first = (idx_t *)calloc(sz, sizeof(idx_t));
    if (!first) return (seq_t)0;  /* Allocation failure */

    seq_t mod = (seq_t)n;

    /* Start with Lucas numbers L(1)=1, L(2)=3 */
    seq_t prev = 1 % mod;      /* L(1) mod n */
    seq_t cur  = 3 % mod;      /* L(2) mod n */

    first[(idx_t)prev] = 1;

    if ((idx_t)cur < sz && first[(idx_t)cur]) {
        seq_t ret = (seq_t)2;
        free(first);
        return ret;
    }
    first[(idx_t)cur] = 2;

    idx_t k = 3;
    while (1) {
        seq_t next = (prev + cur) % mod;
        idx_t idx  = (idx_t)next;
        if (idx < sz && first[idx]) {
            seq_t ret = (seq_t)k;
            free(first);
            return ret;
        }
        first[idx] = k;
        prev = cur;
        cur  = next;
        ++k;
    }
}
