/*
 * A008609  a(n) = n + max_{0 <= i <n} ((n-i)*a(i)), a(0) = 1.
 * https://oeis.org/A008609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(n) = n + max_{0 <= i < n} ((n-i)*a(i))
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 56, u64 n <= 117,
 *            u128 n <= 238, big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n >= 11 it satisfies a(n) = 3*a(n-3) + n, derived from a(n+3) = 3*a(n) + n + 3. */

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* Return the trivial base case without allocating anything. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate an array of size n+1 to store all values up to n. */
    size_t sz = (size_t)n + 1;
    seq_t *arr = alloc_seq(sz);
    if (!arr)
        return (seq_t)0;   /* allocation failed – return 0 */

    arr[0] = (seq_t)1;   /* a(0) */

    /* Compute the first few terms (up to a(10)) by the original definition. */
    for (idx_t i = 1; i <= n && i <= 10; ++i) {
        seq_t max_val = (seq_t)0;
        for (idx_t j = 0; j < i; ++j) {
            seq_t prod = (seq_t)(i - j) * arr[j];
            if (prod > max_val)
                max_val = prod;
        }
        arr[i] = (seq_t)i + max_val;
    }

    /* Use the linear recurrence for all larger n. */
    for (idx_t i = 11; i <= n; ++i) {
        arr[i] = (seq_t)3 * arr[i - 3] + (seq_t)i;
    }

    seq_t result = arr[n];
    free_seq(arr);
    return result;
}
