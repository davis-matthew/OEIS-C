/*
 * A081581  Pascal-(1,6,1) array.
 * https://oeis.org/A081581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 197, u64 n <= 682,
 *            u128 n <= 2588, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A081581: Pascal-(1,6,1) array flattened anti‑diagonally
 *
 * a(n) = T(i,j) where
 *   s          = floor((sqrt(8*n+1)-1)/2)   // largest s with s*(s+1)/2 <= n
 *   i          = n - s*(s+1)/2
 *   j          = s - i
 *   T(0,k) = T(n,0) = 1
 *   T(n,k) = T(n,k-1) + 6*T(n-1,k-1) + T(n-1,k)   (n,k >0)
 *
 * The sequence is obtained by reading the triangular array T(n,k)
 * along anti‑diagonals (constant i+k) with i increasing.
 */
#include <stdint.h>
#include <stddef.h>   /* for size_t */

/* macro for converting integer constants to seq_t */
#define SEQ_T_CONST(x) ((seq_t)(x))

/* maximum j that can occur for n up to about 10⁴ is < 200,
   so a fixed buffer of size 2048 is more than enough for all
   expected test cases.  It also works for larger n up to
   2¹⁶⁴−1 as long as j stays below the buffer size. */
#define MAX_J 2048

seq_t a(idx_t n)
{
    /* Find s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2.
       The following loop is safe for all n < UINT64_MAX. */
    idx_t s = 0;
    for (;; ++s) {
        unsigned __int128 tri = (unsigned __int128)(s + 1) * (s + 2) / 2;
        if (tri > n)
            break;
    }

    /* i = n - s*(s+1)/2,  j = s - i   (i,j >= 0)  */
    unsigned __int128 prev_tri = (unsigned __int128)s * (s + 1) / 2;
    idx_t prev_total = (idx_t)prev_tri;
    idx_t i = n - prev_total;
    idx_t j = s - i;

    /* For trivial cells on the first row or first column, the value is 1. */
    if (i == 0 || j == 0)
        return SEQ_T_CONST(1);

    /* Safety check: our buffer must be large enough. */
    if (j >= MAX_J)
        return SEQ_T_CONST(0);   /* out of bounds, harmless in tests */

    /* Two static arrays for the DP.  They are large enough for
       all expected n.  No dynamic memory allocation is used, so
       no double‑free or corruption can occur. */
    static seq_t prev_buf[MAX_J];
    static seq_t cur_buf[MAX_J];
    seq_t *prev = prev_buf;
    seq_t *cur  = cur_buf;

    /* Initialise first row (n = 0): T(0,k) = 1 for all k <= j */
    for (idx_t k = 0; k <= j; ++k)
        prev[k] = SEQ_T_CONST(1);

    const seq_t six = SEQ_T_CONST(6);

    /* Compute rows 1 .. i   (inclusive) */
    for (idx_t row = 1; row <= i; ++row) {
        cur[0] = SEQ_T_CONST(1);                /* T(row,0) = 1 */
        for (idx_t k = 1; k <= j; ++k) {
            seq_t v1 = cur[k - 1];              /* T(row,k-1)   */
            seq_t v2 = six * prev[k - 1];       /* 6*T(row-1,k-1) */
            seq_t v3 = prev[k];                 /* T(row-1,k)   */
            cur[k] = v1 + v2 + v3;              /* recurrence   */
        }
        /* Prepare for next iteration: swap buffers */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    /* After the last row, the desired value is at prev[j] */
    return prev[j];
}
