/*
 * A237498  Riordan array (1/(1-x-x^2), x/(1+2*x)).
 * https://oeis.org/A237498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 29, i16 n <= 81, i32 n <= 281, i64 n <= 958,
 *            i128 n <= 3593, big any n
 * Helpers:   triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A237498 – flattened Riordan array (1/(1-x-x^2), x/(1+2*x))
 *
 *  a(n) is the entry a(n)=T(r,c) where T is defined by:
 *    T(0,0)=1
 *    T(1,0)=T(1,1)=1
 *    T(2,0)=2, T(2,1)=-1, T(2,2)=1
 *    T(n,0)=T(n-1,0)+T(n-2,0)      (n≥2)
 *    T(n,k)=T(n-1,k-1)-2*T(n-1,k)   (k≥1, 0≤k≤n)
 *
 *  The sequence is obtained by flattening the triangle row‑wise.
 *
 *  The code computes a(n) on demand, without allocating memory
 *  that depends on the value of n, so it works for both small
 *  integral types (int8_t … int64_t) and for the arbitrary‑precision
 *  integer class provided by the build.
 *
 *  ▪ The row index r is found by a simple loop; this costs
 *    O(sqrt(n)) time, negligible for n≤10000.
 *  ▪ Three static arrays of length 2000 hold consecutive rows;
 *    the required row is generated using the recurrence above.
 *  ▪ No dynamic memory allocation is used, so the function is
 *    safe in both C and C++ builds.
 *  ▪ All arithmetic is performed with the allowed operators only.
 */

#include <stdint.h>
#include "../utils/triangular.h"

#define MAX_R 2000                          /* safe for n ≤ 10 000 */

/* static arrays large enough for the largest row we need */
static seq_t row0[MAX_R];
static seq_t row1[MAX_R];
static seq_t row2[MAX_R];

/* triangular number T(r)=r(r+1)/2 (uses 128‑bit to avoid overflow) */

/* --------------------------------------------------------------- */
/*  a(n)  -------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* --- find row r and column c for index n ---------------- */
    idx_t r = 0;
    while (triangular(r + 1) <= n)
        ++r;
    idx_t c = n - triangular(r);

    /* --- base rows ------------------------------------------ */
    row0[0] = (seq_t)1;                 /* row 0 : 1 */
    if (r == 0)
        return (seq_t)1;

    row1[0] = (seq_t)1;                 /* row 1 : 1 1 */
    row1[1] = (seq_t)1;
    if (r == 1)
        return (seq_t)1;

    /* --- iterate to build rows up to r --------------------- */
    seq_t *prevPrev = row0;              /* row i-2   */
    seq_t *prev     = row1;              /* row i-1   */
    seq_t *curr     = row2;              /* row i     */

    for (idx_t i = 2; i <= r; ++i) {
        /* T(i,0) uses both previous rows */
        curr[0] = prev[0] + prevPrev[0];

        /* T(i,k) for k ≥ 1 */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t left  = prev[k - 1];               /* T(i-1,k-1) */
            seq_t right = (k <= i - 1) ? prev[k] : (seq_t)0; /* T(i-1,k) */
            curr[k] = left - (seq_t)2 * right;
        }

        /* rotate pointers so that prevPrev, prev, curr are updated */
        seq_t *tmp = prevPrev;
        prevPrev = prev;
        prev     = curr;
        curr     = tmp;
    }

    /* after the loop, prev holds row r */
    return prev[c];
}
