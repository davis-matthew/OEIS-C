/*
 * A227550  A triangle formed like Pascal's triangle, but with factorial(n) on
 *          the borders instead of 1.
 * https://oeis.org/A227550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227550: triangle with factorial borders
   a(n) is the n-th element in row‑major order of the triangle defined
   by
        T(0,0) = 0!           = 1
        T(n,0) = T(n,n) = n!   (for n >= 1)
        T(n,k) = T(n-1,k-1) + T(n-1,k)   (1 <= k <= n-1)

   The index `n` is 0‑based: a(0)=1, a(1)=1, a(2)=1, a(3)=2, ….
   The routine below computes a(n) directly, building only the
   necessary rows of the triangle.  It works for any seq_t type
   supplied by the build (plain integral or arbitrary‑precision).
   No dynamic allocation is used; static buffers large enough for
   n up to about 10 000 are employed. */

#include <stddef.h>
#include <stdint.h>

/* maximum row length needed for n <= 10000 is 143; we use a small safety margin */
#define MAX_ROW_LEN 200

seq_t a(idx_t n)
{
    /* Find the row r containing the entry a(n).
       r satisfies: r(r+1)/2 <= n < (r+1)(r+2)/2.  */
    idx_t r = 0;
    idx_t low  = 0, high = MAX_ROW_LEN;            /* high is an upper bound on r */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 left  = (unsigned __int128)mid  * (mid + 1) / 2;
        unsigned __int128 right = (unsigned __int128)(mid + 1) * (mid + 2) / 2;
        if (left <= n && n < right) { r = mid; break; }
        if (n < left)  { high = mid - 1; }
        else           { low = mid + 1; }
    }

    /* Position inside the row (0‑based). */
    unsigned __int128 start_of_row = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = (idx_t)(n - start_of_row);

    /* Static buffers to avoid dynamic allocation.  Two buffers alternate
       while we build the triangle row by row. */
    static seq_t bufA[MAX_ROW_LEN];
    static seq_t bufB[MAX_ROW_LEN];
    seq_t *prev = bufA;   /* will hold the previous row */
    seq_t *cur  = bufB;   /* will hold the current row  */

    /* Initialise the first row (n = 0). */
    prev[0] = (seq_t)1;   /* T(0,0) = 0! = 1 */

    /* Factorial accumulator: fact = i! for the current row i. */
    seq_t fact = (seq_t)1;

    /* Build rows 1 .. r. */
    for (idx_t i = 1; i <= r; ++i) {
        fact = fact * (seq_t)i;          /* i!   */
        idx_t len = i + 1;               /* number of entries in row i */

        cur[0] = fact;                   /* left border */
        for (idx_t j = 1; j < i; ++j) {  /* interior entries */
            cur[j] = prev[j-1] + prev[j];
        }
        cur[i] = fact;                   /* right border */

        /* Prepare for next iteration.  Swap the buffers. */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    /* After the loop, `prev` holds row `r`.  Return the k‑th element. */
    return prev[k];
}
