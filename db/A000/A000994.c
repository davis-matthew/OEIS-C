/*
 * A000994  Shifts 2 places left under binomial transform.
 * https://oeis.org/A000994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, eigen
 * Formula:   a(n) = Sum_{k=0..n-2} binomial(n-2,k)*a(k), a(0) = 1, a(1) = 0.
 *            Computed with an Aitken/Bell-style addition triangle: T(m,0) =
 *            a(m), T(m,j) = T(m,j-1) + T(m-1,j-1), and a(m+2) = T(m,m)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 31, u128 n <= 51,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 647 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *a306_alloc0(idx_t len)
{
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *a306_grow(seq_t *old, idx_t oldlen, idx_t newlen)
{
    seq_t *p = a306_alloc0(newlen);
    for (idx_t i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) mem_free(old);
    return p;
}

static seq_t *A = 0;     /* known terms a(0..have-1) */
static seq_t *R = 0;     /* current triangle row T(rowm, 0..rowm) */
static idx_t have = 0;
static idx_t rowm = 0;
static idx_t cap = 0;

seq_t a(idx_t n) {
    idx_t j;
    if (cap == 0) {
        cap = 64;
        A = a306_alloc0(cap);
        R = a306_alloc0(cap);
        A[0] = 1;
        A[1] = 0;
        have = 2;
        R[0] = 1;   /* row 0: T(0,0) = a(0) */
        rowm = 0;
    }
    if (n < have) return A[n];
    if (n + 2 > cap) {
        idx_t nc = cap;
        while (nc < n + 2) nc *= 2;
        A = a306_grow(A, cap, nc);
        R = a306_grow(R, cap, nc);
        cap = nc;
    }
    while (have <= n) {
        /* invariant: have == rowm + 2 */
        A[have] = R[rowm];
        have++;
        /* advance row rowm -> rowm+1 in place */
        {
            seq_t carry = R[0];
            R[0] = A[rowm + 1];
            for (j = 1; j <= rowm + 1; j++) {
                seq_t t = (j <= rowm) ? R[j] : (seq_t)0;
                R[j] = R[j - 1] + carry;
                carry = t;
            }
            rowm++;
        }
    }
    return A[n];
}
