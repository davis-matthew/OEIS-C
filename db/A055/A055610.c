/*
 * A055610  A companion sequence to A011896.
 * https://oeis.org/A055610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-7) - 3*a(n-8) +
 *            3*a(n-9) - a(n-10) (n ≥ 10), with initial values a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=1, a(4)=2, a(5)=5, a(6)=9, a(7)=15, a(8)=24,
 *            a(9)=36
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Handle the first 10 terms directly. */
    if (n <= 9) {
        static const seq_t init[10] = {
            (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1,
            (seq_t)2, (seq_t)5, (seq_t)9, (seq_t)15,
            (seq_t)24, (seq_t)36
        };
        return init[n];
    }

    /* Allocate an array for all terms up to n. */
    seq_t *v = seq_alloc_new_array(n + 1);
    if (!v) {                 /* allocation failure – return 0 */
        return (seq_t)0;
    }

    /* Initialise the known first ten terms. */
    static const seq_t init[10] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1,
        (seq_t)2, (seq_t)5, (seq_t)9, (seq_t)15,
        (seq_t)24, (seq_t)36
    };
    for (idx_t i = 0; i < 10; ++i) v[i] = init[i];

    /* Compute all subsequent terms using the linear recurrence. */
    for (idx_t i = 10; i <= n; ++i) {
        seq_t t1 = v[i-1] * (seq_t)3;
        seq_t t2 = v[i-2] * (seq_t)3;
        seq_t t3 = v[i-3];
        seq_t t4 = v[i-7];
        seq_t t5 = v[i-8] * (seq_t)3;
        seq_t t6 = v[i-9] * (seq_t)3;
        seq_t t7 = v[i-10];
        v[i] = t1 - t2 + t3 + t4 - t5 + t6 - t7;
    }

    seq_t result = v[n];
    seq_free_array(v);
    return result;
}
