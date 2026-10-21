/*
 * A097139  Convolution of 5^n and floor(n/2).
 * https://oeis.org/A097139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + (5^(n-1)-1)/4 with a(0) = 0, a(1) = 0. Auxiliary
 *            sequence b(n) = (5^n-1)/4 satisfies b(0) = 0, b(1) = 1, b(n) =
 *            5*b(n-1) + 1. Then a(n) = a(n-2) + b(n-1) for n ≥ 2. Sequences
 *            are cached so that calls in any order are efficient
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *A = NULL;   /* cache for a(n) */
static seq_t *B = NULL;   /* auxiliary b(n) = (5^n-1)/4 */
static size_t sz = 0;     /* current allocated size */

/* Expand cached storage to at least newlen entries, keeping old data */
static void expand_to(size_t newlen)
{
    if (newlen <= sz)
        return;
    seq_t *newA = seq_alloc_new_array(newlen);
    seq_t *newB = seq_alloc_new_array(newlen);
    if (A) {
        memcpy(newA, A, sz * sizeof(seq_t));
        seq_free_array(A);
    }
    if (B) {
        memcpy(newB, B, sz * sizeof(seq_t));
        seq_free_array(B);
    }
    A = newA;
    B = newB;
    sz = newlen;
}

/* Ensure that values up to n are computed and stored */
static void ensure_up_to(idx_t n)
{
    if ((size_t)(n + 1) > sz) {
        size_t old_sz = sz;
        expand_to((size_t)(n + 1));
        for (idx_t i = (idx_t)old_sz; i <= n; ++i) {
            if (i == 0) {          /* base values */
                B[0] = 0;
                A[0] = 0;
            } else if (i == 1) {
                B[1] = 1;          /* (5^1-1)/4 = 1 */
                A[1] = 0;
            } else {
                B[i] = 5 * B[i - 1] + 1;  /* b(i) = 5*b(i-1)+1 */
                A[i] = A[i - 2] + B[i - 1]; /* a(i) = a(i-2)+b(i-1) */
            }
        }
    }
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return A[n];
}
