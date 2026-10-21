/*
 * A065506  Number of (binary) bit strings of length n having an even length
 *          block of 0's followed by an odd length block of 1's.
 * https://oeis.org/A065506
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=2*a(n-1)+3*a(n-2)-5*a(n-3)-2*a(n-4), a(0)=a(1)=a(2)=0,
 *            a(3)=1 (for n≥4)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *A = NULL;
    static idx_t  last_computed = 0;   /* highest index already computed */
    static size_t capacity = 0;        /* allocated array size */

    /* --- allocate / enlarge cache if needed -------------------------------- */
    if (n >= (idx_t)capacity) {
        size_t newcap = (capacity == 0) ? (n + 1) : (capacity * 2);
        if (newcap < (size_t)(n + 1)) newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failed */
        A = tmp;
        /* initialise newly allocated entries to 0 */
        for (size_t i = capacity; i < newcap; ++i) A[i] = 0;
        capacity = newcap;
    }

    /* --- compute values up to n --------------------------------------------- */
    if (last_computed < n) {
        /* initialise bases for indices 0..3 if they were not yet set */
        if (last_computed < 3) {
            for (idx_t i = last_computed + 1; i <= 3 && i <= n; ++i) {
                A[i] = (i == 3) ? (seq_t)1 : (seq_t)0;
            }
        }
        /* apply the recurrence for i >= 4 */
        for (idx_t i = (last_computed > 3 ? last_computed + 1 : 4);
             i <= n; ++i) {
            A[i] = 2 * A[i - 1] + 3 * A[i - 2] - 5 * A[i - 3] - 2 * A[i - 4];
        }
        last_computed = n;
    }

    return A[n];
}
