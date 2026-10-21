/*
 * A221796  Number of n X 3 arrays of occupancy after each element moves to
 *          some horizontal or vertical neighbor, without move-in move-out
 *          straight through or left turns.
 * https://oeis.org/A221796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-2) + 14*a(n-3) - 165*a(n-4) - 20*a(n-5) +
 *            329*a(n-6) - 48*a(n-7) - 131*a(n-8) - 34*a(n-9) + 9*a(n-10),
 *            a(1)=0, a(2)=12, a(3)=12, a(4)=207, a(5)=470, a(6)=4073,
 *            a(7)=13314, a(8)=85974, a(9)=338148, a(10)=1892213
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case first – no allocation needed. */
    if (n == 1) return (seq_t)0;

    /* The first ten values as given in the OEIS entry. */
    static const seq_t baseVals[10] = {
        (seq_t)0,          /* a(1) */
        (seq_t)12,         /* a(2) */
        (seq_t)12,         /* a(3) */
        (seq_t)207,        /* a(4) */
        (seq_t)470,        /* a(5) */
        (seq_t)4073,       /* a(6) */
        (seq_t)13314,      /* a(7) */
        (seq_t)85974,      /* a(8) */
        (seq_t)338148,     /* a(9) */
        (seq_t)1892213     /* a(10) */
    };

    /* Allocate an array large enough to hold all terms up to n.  */
    size_t N = (size_t)n;
    seq_t *A = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!A) return (seq_t)0;   /* allocation failure – unlikely for small n */

    /* Load the known initial values. */
    for (size_t i = 1; i <= (size_t)10 && i <= N; ++i)
        A[i] = baseVals[i - 1];

    /* For n > 10 use the empirical recurrence. */
    if (N > 10) {
        for (size_t i = 11; i <= N; ++i) {
            seq_t pos = (seq_t)27 * A[i - 2]
                        + (seq_t)14 * A[i - 3]
                        + (seq_t)329 * A[i - 6]
                        + (seq_t)9  * A[i - 10];
            seq_t neg = (seq_t)165 * A[i - 4]
                        + (seq_t)20  * A[i - 5]
                        + (seq_t)48  * A[i - 7]
                        + (seq_t)131 * A[i - 8]
                        + (seq_t)34  * A[i - 9];
            A[i] = pos - neg;   /* pos >= neg for all n ≥ 11 */
        }
    }

    seq_t result = A[N];
    free(A);
    return result;
}
