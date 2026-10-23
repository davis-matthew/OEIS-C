/*
 * A187778  Numbers k dividing psi(k), where psi is the Dedekind psi function
 *          (A001615).
 * https://oeis.org/A187778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1; a(n) = 6 * A003586(n-1) for n > 1, where
 *            A003586(k) is the k‑th number of the form 2^i·3^j (i,j≥0) sorted
 *            increasingly
 * Range:     u8 n <= 15, u16 n <= 69, u32 n <= 298, u64 n <= 1241,
 *            u128 n <= 5065, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* We need the (n‑1)-th member of A003586. */
    idx_t m = n - 1;               /* number of terms needed after the initial 1 */
    seq_t *arr = (seq_t *)malloc(sizeof(seq_t) * m);
    if (!arr)
        return 0;                  /* out of memory: return 0 as a safe fallback */

    arr[0] = (seq_t)1;             /* first term of A003586 */
    idx_t i2 = 0, i3 = 0;          /* indices for multiples by 2 and 3 */

    for (idx_t k = 1; k < m; ++k) {
        seq_t next2 = arr[i2] * (seq_t)2;
        seq_t next3 = arr[i3] * (seq_t)3;
        seq_t nxt = next2 < next3 ? next2 : next3;
        arr[k] = nxt;
        if (nxt == next2) i2++;
        if (nxt == next3) i3++;
    }

    seq_t result = arr[m - 1] * (seq_t)6;
    free(arr);
    return result;
}
