/*
 * A135837  A007318 * a triangle with (1, 2, 2, 4, 4, 8, 8, ...) in the main
 *          diagonal and the rest zeros.
 * https://oeis.org/A135837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, tabl
 * Formula:   a(n) is the n‑th non‑zero term of the triangle defined by the
 *            recurrence T(n,k) = 2*T(n-1,k) - T(n-2,k) + 2*T(n-2,k-2), with
 *            T(0,0)=T(1,0)=T(2,0)=1, T(1,1)=T(2,2)=0, T(2,1)=2 and T(n,k)=0
 *            for k<0 or k>n. The sequence enumerates these values row by row
 *            (triangle numbers)
 * Range:     u8 n <= 40, u16 n <= 127, u32 n <= 393, u64 n <= 1460,
 *            u128 n <= 5515, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximum row that may be needed for n <= 10000 */
#define MAX_R 200

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1 */

    /* Find the row r that contains the nth element (1‑based). */
    idx_t r = 1, cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }
    /* Position within the row (1‑based). */
    idx_t k = n - (cum - r);

    /* Handle the trivial first two rows directly. */
    if (r == 1) return 1ul;                 /* T(0,0) */
    if (r == 2) {
        return (k == 1) ? 1ul : 2ul;        /* T(1,0) and T(1,1) */
    }

    /* Three static arrays used to hold consecutive rows.
       The size is constant (MAX_R+1) so the code is valid in C and C++. */
    seq_t row0[MAX_R + 1] = {0};
    seq_t row1[MAX_R + 1] = {0};
    seq_t row2[MAX_R + 1] = {0};

    /* Pointers for easier rotation of rows. */
    seq_t *prevprev = row0;   /* row i-2   */
    seq_t *prev    = row1;   /* row i-1   */
    seq_t *cur     = row2;   /* row i     */

    /* Initialise the first two non‑zero rows. */
    prevprev[1] = 1;          /* row 1 : T(0,0)   */
    prev[1]    = 1;           /* row 2 : T(1,0)   */
    prev[2]    = 2;           /*           T(1,1) */

    /* Compute rows 3 … r using the recurrence. */
    for (idx_t i = 3; i <= r; ++i) {
        for (idx_t j = 1; j <= i; ++j) {
            seq_t t1 = (j <= i - 1) ? prev[j] : 0;
            seq_t t2 = (j <= i - 2) ? prevprev[j] : 0;
            seq_t t3 = (j >= 3)     ? prevprev[j - 2] : 0;
            cur[j] = (t1 << 1) - t2 + (t3 << 1);
        }
        /* Rotate the three rows for the next iteration. */
        if (i < r) {
            seq_t *tmp = prevprev;
            prevprev  = prev;
            prev      = cur;
            cur       = tmp;
        }
    }

    /* After the loop, 'cur' holds the required row. */
    return cur[k];
}
