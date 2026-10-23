/*
 * A124959  Triangle read by rows: T(n,k) = a(k)*binomial(n,k) (0 <= k <= n),
 *          where a(0)=1, a(1)=2, a(k) = a(k-1) + 3*a(k-2) for k >= 2 (a(k) =
 *          A006138(k)).
 * https://oeis.org/A124959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 72, u32 n <= 242, u64 n <= 805,
 *            u128 n <= 3051, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the nth term of OEIS A124959, the flattened triangle
   read by rows: T(r,k) = a(k) · binomial(r,k) for 0 ≤ k ≤ r.
   Here a(0)=1, a(1)=2, a(k)=a(k−1)+3·a(k−2) for k≥2.
   The overall sequence a(n) lists the triangle entries in row‐major order.
*/

#include <stdlib.h>
#include <stdint.h>

/* Return binomial(n,k) as seq_t.  Works for any seq_t implementation. */
static seq_t local_binomial(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply then divide to keep result integral */
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

/* Compute the nth term of the flattened triangle. */
seq_t a(idx_t n)
{
    /* ------------ find row r and column c ------------- */
    idx_t r = 0;
    /* find largest r such that (r+1)(r+2)/2 <= n + 1  */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    /* Now r is the row containing index n */
    idx_t start = r * (r + 1) / 2;   /* first index of this row */
    idx_t c = n - start;             /* column within the row */

    /* ------------ compute a(c) using recurrence ------------ */
    static seq_t *acache = NULL;
    static size_t acap = 0;
    static idx_t anum = 0;
    if (acap == 0) {
        acap = 2;
        acache = (seq_t *)malloc(acap * sizeof(seq_t));
        acache[0] = (seq_t)1;   /* a(0) */
        acache[1] = (seq_t)2;   /* a(1) */
        anum = 2;
    }
    while (anum <= c) {
        seq_t next = acache[anum - 1] + (seq_t)3 * acache[anum - 2];
        if (anum >= acap) {
            acap *= 2;
            acache = (seq_t *)realloc(acache, acap * sizeof(seq_t));
        }
        acache[anum++] = next;
    }
    seq_t ak = acache[c];

    /* ------------ compute binomial(r,c) ------------------- */
    seq_t bin = local_binomial(r, c);

    /* ------------ final value ------------------------------ */
    return ak * bin;
}
