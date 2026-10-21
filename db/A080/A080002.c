/*
 * A080002  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={-1,0,1}.
 * https://oeis.org/A080002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 48, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A080002: Number of permutations satisfying
 * -2 ≤ p(i)-i ≤ 3 and p(i)-i ∉ {-1,0,1} for i=1..n.
 *
 * Sequence satisfies the linear recurrence
 *   a(n) = a(n-2)+a(n-4)+2*a(n-5)-a(n-6)-a(n-7)-a(n-10)   (n ≥ 10)
 * with initial values
 *   a(0)=1, a(1)=0, a(2)=0, a(3)=0,
 *   a(4)=1, a(5)=1, a(6)=0, a(7)=0,
 *   a(8)=1, a(9)=3.
 *
 * The function computes a(n) for any n ≥ 0 using this recurrence.
 * It allocates a temporary array big enough for the required indices
 * and frees it before returning.  The implementation avoids using
 * seq_free_array, which is not defined in this context.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"

seq_t a(idx_t n) {
    /* Determine array size: we need indices 0..max(n,9). */
    size_t sz = (size_t)((n >= 9) ? n + 1 : 10);
    seq_t *arr = seq_alloc_new_array(sz);

    /* Initialize base values a(0)..a(9). */
    arr[0] = (seq_t)1;
    arr[1] = (seq_t)0;
    arr[2] = (seq_t)0;
    arr[3] = (seq_t)0;
    arr[4] = (seq_t)1;
    arr[5] = (seq_t)1;
    arr[6] = (seq_t)0;
    arr[7] = (seq_t)0;
    arr[8] = (seq_t)1;
    arr[9] = (seq_t)3;

    /* If the requested n is within the precomputed range, finish. */
    if (n < 10) {
        seq_t result = arr[n];
        free(arr);
        return result;
    }

    /* Apply the recurrence for all n ≥ 10. */
    for (idx_t i = 10; i <= n; ++i) {
        seq_t term = arr[i - 2]          /* a(i-2) */
                   + arr[i - 4]          /* a(i-4) */
                   + (arr[i - 5] << 1); /* 2*a(i-5) */
        term = term
             - arr[i - 6]              /* a(i-6) */
             - arr[i - 7]              /* a(i-7) */
             - arr[i -10];             /* a(i-10) */
        arr[i] = term;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
