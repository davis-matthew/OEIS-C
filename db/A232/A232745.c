/*
 * A232745  Numbers k for which the largest m such that m! divides k is even.
 * https://oeis.org/A232745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 9707, u32 n <= 9707, u64 n <= 9707,
 *            u128 n <= 9707, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS A232745 sequence.
   Numbers k for which the largest m with m! | k is even.
   The function generates terms on demand, memoising the result
   in a static array to handle arbitrary call order. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state */
    static seq_t *seq = NULL;      /* array of found terms   */
    static idx_t seq_len = 0;      /* number of stored terms */
    static idx_t next_k = 1;       /* next integer to test   */
    static size_t cap = 0;         /* current allocated size */

    /* initialise the array on first use */
    if (seq == NULL) {
        cap = 128;
        seq = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        idx_t k = next_k++;          /* candidate integer */

        /* find the largest m such that m! divides k */
        idx_t e = 1;                 /* at least 1! divides every integer */
        unsigned long long fact = 1; /* compute factorials in 64-bit up to k */

        for (idx_t m = 2; ; ++m) {
            /* avoid overflow: if fact > k/m then fact*m > k (or overflow) */
            if (fact > k / m) break;
            fact *= m;
            if (k % (idx_t)fact == 0) {
                e = m;                 /* keep track of the largest m */
            } else {
                break;                 /* further factorials cannot divide */
            }
        }

        /* include k if e is even */
        if ((e & 1u) == 0) {
            if (seq_len >= cap) {        /* grow the array when needed */
                cap *= 2;
                seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)k;
        }
    }

    /* return the requested term (1-based index) */
    return seq[n - 1];
}
