/*
 * A092686  Triangle, read by rows, such that the convolution of each row with
 *          {1,2} produces a triangle which, when flattened, equals this
 *          flattened form of the original triangle.
 * https://oeis.org/A092686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the flattened triangle defined by
 *            T(0,0) = 1 T(n,k) = 2*T(n‑1,k) + T(n‑1,k+1) (T(n‑1,k+1) = 0 for
 *            k+1 > n‑1) T(n,n) = T(n,0) giving rows : 1; 2,2; 6,4,6;
 *            16,14,12,16;
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 252, u64 n <= 495,
 *            u128 n <= 495, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* ----------------------------------------------------------
       Determine the row index r such that offset(r) = r*(r+1)/2 <= n
       ---------------------------------------------------------- */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 prod = (__int128)mid * (__int128)(mid + 1) / 2;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* offset of the start of row r */
    __int128 off128 = (__int128)r * (__int128)(r + 1) / 2;
    idx_t offset = (idx_t)off128;
    idx_t col = n - offset;          /* column within the row */

    /* ----------------------------------------------------------
       Build rows up to r using two temporary arrays
       ---------------------------------------------------------- */
    size_t maxlen = (size_t)(r + 1);
    seq_t *prev = (seq_t*)malloc(sizeof(seq_t) * maxlen);
    seq_t *curr = (seq_t*)malloc(sizeof(seq_t) * maxlen);
    if (!prev || !curr) {
        /* allocation failed – return 0 as a safe fallback */
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    prev[0] = (seq_t)1;   /* row 0 */

    for (idx_t i = 1; i <= r; ++i) {
        /* compute row i (length i+1) into curr */
        for (idx_t k = 0; k < i; ++k) {
            seq_t left  = prev[k];
            seq_t right = (k + 1 < i) ? prev[k + 1] : (seq_t)0;
            curr[k] = left + left + right;   /* 2*prev[k] + prev[k+1] */
        }
        curr[i] = curr[0];   /* wrap: T(i,i) = T(i,0) */

        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = (r == 0) ? prev[0] : prev[col];

    free(prev);
    free(curr);
    return result;
}
