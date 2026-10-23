/*
 * A180711  a(n) = [x^n] A(x) = floor(sqrt( b(n) )) where b(n) = [x^n] A(x)^2.
 * https://oeis.org/A180711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊sqrt( Σ_{k=0}^n a(k)*a(n-k) )⌋ The sequence is defined
 *            by a(0)=1 and for n>0 a(n) satisfies m² ≤ 2m+S < (m+1)², where S
 *            = Σ_{k=1}^{n-1} a(k)*a(n-k). This implementation computes a(n)
 *            by iterating this inequality
 * Range:     u8 n <= 11, u16 n <= 44, u32 n <= 286, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* Maximum n the caller may request. Adjust only if a larger range
   is required. */
#define MAX_N 20000

static seq_t A[MAX_N + 1];
static idx_t A_len = 0;   /* index of the highest computed term */

/* Compute a(n) from the partial sum S = Σ_{k=1}^{n-1} a(k)*a(n-k).
   All arithmetic is performed using seq_t, so the code works
   for fixed‑width integer types as well as arbitrary‑precision
   integer classes. */
static seq_t compute_a_from_sum(idx_t n, seq_t sum)
{
    seq_t m = A[n - 1];            /* start from the previous term */

    /* Iterate until m² ≤ 2m+sum < (m+1)² holds. */
    while (1) {
        seq_t val   = sum + ((seq_t)2) * m;
        seq_t left  = m * m;
        seq_t right = (m + ((seq_t)1)) * (m + ((seq_t)1));

        if (left <= val && val < right)
            break;                 /* found the correct m */

        if (val < left)            /* m is too large */
            --m;
        else                        /* val >= right, m too small */
            ++m;
    }
    return m;
}

/* Public interface required by the harness. */
seq_t a(idx_t n)
{
    if (n > MAX_N)
        return (seq_t)0;   /* out of bounds – caller must not exceed this limit */

    if (A_len == 0) {          /* initialise the first term */
        A[0] = (seq_t)1;
        A_len = 0;
    }

    if (n <= A_len)
        return A[n];

    /* Compute terms up to index n */
    for (idx_t i = A_len + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < i; ++k)
            sum += A[k] * A[i - k];

        A[i] = compute_a_from_sum(i, sum);
    }
    A_len = n;
    return A[n];
}
