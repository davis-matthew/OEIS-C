/*
 * A143038  Triangle read by rows, A000012 * A134309 * A000012, where A134309
 *          = an infinite lower triangular matrix with (1, 1, 2, 4, 8, 16,
 *          ...) in the main diagonal and the rest zeros.
 * https://oeis.org/A143038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^i - 2^(i-1-j) for j < i, otherwise a(n) = 2^i, where i
 *            = floor((sqrt(8*n+1)-1)/2) and j = n - i(i+1)/2
 * Range:     u8 n <= 43, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow2_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_custom.h"

/* Return the n-th term of sequence A143038. */
seq_t a(idx_t n)
{
    /* Find i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2. */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;

    idx_t offset = i * (i + 1) / 2;  /* first index of row i */
    idx_t j = n - offset;            /* column index within the row */

    seq_t val = pow2_custom(i);      /* base 2^i */

    if (j < i) {                     /* subtract lower power */
        seq_t sub = pow2_custom(i - 1 - j);
        val -= sub;
    }

    return val;
}
