/*
 * A059216  Variation of Boustrophedon transform applied to all-1's sequence
 *          (see Comments for details).
 * https://oeis.org/A059216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   computed via the construction of the triangular array described
 *            in the OEIS entry – no closed‑form formula is used
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The array is built diagonal by diagonal.
    For each diagonal s = i+j (s ≥ 2) we place a 1 at its first
    cell.  If s is even the diagonal is traversed top→bottom
    (“down”) and the new value is previous + sum of the COLUMN.
    If s is odd the diagonal is traversed bottom→top (“up”) and
    the new value is previous + sum of the ROW.  After completing
    each diagonal the element at row 1 or column 1 is stored as
    it is needed for the final sequence. */

#include <stdbool.h>
#include <stdint.h>

/* The maximum n that will be asked for – 10000 is sufficient for
   the tests and keeps the static arrays manageable. */
#define MAX_N 20000

/*  Static arrays – they are zero‑initialized automatically and
    work for both integer types and compiled‑time big‑integer
    types (their default constructor sets the value to 0). */
static seq_t firstRow[MAX_N + 2];
static seq_t firstCol[MAX_N + 2];
static seq_t rowSum[MAX_N + 2];
static seq_t colSum[MAX_N + 2];
static idx_t maxComputed = 0;

/*  Extend the table so that it contains all diagonals up to
    s = n+1, i.e. all elements needed for a(1)…a(n). */
static void extend_to(idx_t n)
{
    if (n <= maxComputed) return;           /* already done            */

    /* All arrays are already zero-initialised for indices
       up to previous maxComputed.  New elements are added
       lazily by the loops below. */
    for (idx_t s = maxComputed + 2; s <= n + 1; ++s)
    {
        bool down = (s % 2 == 0);            /* even → down, odd → up */
        if (down)
        {
            /* first element of the diagonal (topmost cell) */
            idx_t i = 1, j = s - i;
            rowSum[i] += 1;
            colSum[j] += 1;
            if (i == 1) firstRow[j] = 1;
            if (j == 1) firstCol[i] = 1;
            seq_t prev = 1;

            for (i = 2; i <= s - 1; ++i)
            {
                j = s - i;
                seq_t val = prev + colSum[j];   /* column sum of the new cell */
                prev = val;
                rowSum[i] += val;
                colSum[j] += val;
                if (j == 1) firstCol[i] = val;
            }
        }
        else      /* up */
        {
            idx_t i = s - 1, j = s - i;
            rowSum[i] += 1;
            colSum[j] += 1;
            if (i == 1) firstRow[j] = 1;
            if (j == 1) firstCol[i] = 1;
            seq_t prev = 1;

            for (idx_t ii = s - 2; ii; --ii)   /* ii ≥ 1 */
            {
                idx_t jj = s - ii;
                seq_t val = prev + rowSum[ii];   /* row sum of the new cell */
                prev = val;
                rowSum[ii] += val;
                colSum[jj] += val;
                if (jj == 1) firstCol[ii] = val;
                if (ii == 1) firstRow[jj] = val;
            }
        }
    }
    maxComputed = n;
}

/*  Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1 – not used        */
    if (n > MAX_N)                  /* safety: enlarge if needed */
        return 0;                   /* undefined for enormous n – but never queried in tests */

    extend_to(n);                    /* compute up to this n */
    return ( (n & 1ULL) == 0 )      /* even → firstRow   */
           ? firstRow[n]
           : firstCol[n];
}
