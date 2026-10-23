/*
 * A173536  A173039(n) + A173259(n).
 * https://oeis.org/A173536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 194, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A173536: a(n) = 3*a(n-3) - 3*a(n-6) + a(n-9)  
 * initial values: a(1)=1, a(2)=0, a(3)=1, a(4)=9, a(5)=4, a(6)=25,
 * a(7)=49, a(8)=16, a(9)=81 */

#include <stddef.h>
#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A173536. */
seq_t a(idx_t n)
{
    /* Initially known 9 terms */
    static const seq_t init[9] = {
        (seq_t)1ULL, (seq_t)0ULL, (seq_t)1ULL,
        (seq_t)9ULL, (seq_t)4ULL, (seq_t)25ULL,
        (seq_t)49ULL,(seq_t)16ULL,(seq_t)81ULL
    };

    if (n <= 9)
        return init[n - 1];

    /* Allocate an array of size n+1 (indices start at 1).  */
    size_t sz = (size_t)n + 1;
    seq_t *a = (seq_t *)calloc(sz, sizeof(seq_t));
    if (a == NULL)
        abort();                     /* allocation failure */

    /* Load the known initial terms. */
    for (idx_t i = 1; i <= 9; ++i)
        a[i] = init[i - 1];

    /* Compute remaining terms with the recurrence. */
    for (idx_t i = 10; i <= n; ++i)
        a[i] = 3 * a[i - 3] - 3 * a[i - 6] + a[i - 9];

    seq_t result = a[n];
    free(a);
    return result;
}
