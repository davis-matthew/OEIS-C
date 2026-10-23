/*
 * A152237  A modulo two parity function as a triangle sequence:k=1;
 *          t(n,m)=Binomial[n,m]+p(n,m); Always even parity function:
 *          p(n,m)=If[Mod[Binomial[n, m], 2] == 0, 2^(k - 1)*Binomial[n, m],
 *          If[Mod[Binomial[n, m], 2] == 1 && Binomial[n, m] > 1, 2^k*
 *          Binomial[n, m], 0]].
 * https://oeis.org/A152237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152237
   Definition:   t(n,m) = Binomial[n,m] + p(n,m)
   where k = 1 and

   p(n,m) = { Binomial[n,m]          if Binomial[n,m] is even
             { 2*Binomial[n,m]        if Binomial[n,m] > 1 is odd
             { 0                      if Binomial[n,m] = 1

   The sequence a(n) lists the entries t(n,m) row‑wise:
   a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=4, … (see OEIS A152237).  */

#include <stdint.h>
#include "../utils/binomial.h"

/* Convert a flat index n into (row r, column m) of the triangle. */
static void index_to_rowcol(idx_t n, idx_t *r, idx_t *m)
{
    idx_t low = 0, high = n, best = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= (unsigned __int128)n) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    *r = best;
    unsigned __int128 prev_tri = (unsigned __int128)best * (best + 1) / 2;
    *m = n - (idx_t)prev_tri;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t r, m;
    index_to_rowcol(n, &r, &m);

    seq_t bc = binomial(r, m);   /* Binomial coefficient. */

    /* Binomial = 1 (endpoints). */
    if (bc == (seq_t)1)
        return (seq_t)1;

    /* Multiply by 2 (default). Add an extra term if binomial is odd. */
    seq_t res = bc + bc;         /* 2 * bc  */
    if ((bc & (seq_t)1) != (seq_t)0)
        res += bc;               /* 3 * bc   */

    return res;
}
