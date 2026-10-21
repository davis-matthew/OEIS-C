/*
 * A239366  Triangular array read by rows: T(n,k) is the number of palindromic
 *          compositions of n having exactly k 1's, n>=0, 0<=k<=n.
 * https://oeis.org/A239366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n -
 *            r*(r+1)/2. T(n,k) counts palindromic compositions of n having
 *            exactly k 1's. Recurrence: T(0,0)=1; T(1,1)=1. For n>1:
 *            T(n,0)=1. For each x=1..⌊n/2⌋: inner = n-2*x delta = (x==1)?2:0
 *            // two outer 1's add two ones For each k0=0..inner:...
 * Range:     u8 n <= 254, u16 n <= 746, u32 n <= 2639, u64 n <= 9343,
 *            u128 n <= 9480, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* static memoisation table */
static seq_t **T = NULL;
static idx_t max_T = 0;

/* Ensure that rows 0..limit are computed and stored in T. */
static void ensure_T(idx_t limit)
{
    /* Allocate or enlarge the row pointer array. */
    if (T == NULL) {
        T = (seq_t **)calloc((size_t)(limit + 1), sizeof(seq_t *));
    } else {
        T = (seq_t **)realloc(T, (size_t)(limit + 1) * sizeof(seq_t *));
    }

    /* Compute new rows sequentially. */
    for (idx_t n = max_T + 1; n <= limit; ++n) {
        /* Allocate each row with zeroed entries. */
        T[n] = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));

        /* Base cases. */
        if (n == 1) {
            T[n][1] = (seq_t)1;
        } else if (n > 1) {
            T[n][0] = (seq_t)1;
        }

        /* Outer part contributions. */
        for (idx_t x = 1; x <= n / 2; ++x) {
            idx_t inner = n - 2 * x;
            idx_t delta = (x == 1) ? 2 : 0;
            for (idx_t k0 = 0; k0 <= inner; ++k0) {
                seq_t val = T[inner][k0];
                if (val != (seq_t)0) {
                    T[n][k0 + delta] += val;
                }
            }
        }
    }

    /* Initialise row 0 if it has not been created yet. */
    if (max_T == 0 && T[0] == NULL) {
        T[0] = (seq_t *)calloc(1, sizeof(seq_t));
        T[0][0] = (seq_t)1;
    }

    max_T = limit;
}

/* Return the n-th term of the flattened sequence. */
seq_t a(idx_t n)
{
    /* Find row r and column k corresponding to linear index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    idx_t front = r * (r + 1) / 2;
    idx_t k = n - front;

    ensure_T(r);

    return T[r][k];
}
