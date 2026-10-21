/*
 * A238206  Square array T(n,k), n>=0, k>=0, read by antidiagonals, where
 *          T(0,k) is A007494(k) and T(n,k) = 3*T(n-1,k) + 1 for n>0.
 * https://oeis.org/A238206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) read by antidiagonals from T(i,j) = T(0,j)*3^i + (3^i-1)/2,
 *            where T(0,j) = (6*j + 1 - (-1)^j)/4 and i = d, j = s - d with s
 *            = floor((sqrt(8*n+1)-1)/2), d = n - s*(s+1)/2
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find diagonal index s and offset */
    idx_t s = 0;          /* diagonal number */
    idx_t offset = 0;     /* index of first element of this diagonal */
    while (n >= offset + (s + 1)) {
        offset += s + 1;
        s++;
    }
    /* Position within diagonal */
    idx_t d = n - offset;   /* 0 <= d <= s */
    idx_t i = d;            /* row index */
    idx_t j = s - d;        /* column index */

    /* Compute 3^i */
    seq_t pow3 = 1;
    for (idx_t k = 0; k < i; ++k)
        pow3 *= 3;

    /* Compute T(0,j) = (6*j + 1 - (-1)^j)/4 */
    seq_t j_seq = (seq_t)j;
    seq_t T0j;
    if (j & 1) {   /* j odd : (-1)^j = -1, 1 - (-1) = 2 */
        T0j = ((6 * j_seq + 2) / 4);
    } else {       /* j even : (-1)^j = 1, 1 - 1 = 0 */
        T0j = ((6 * j_seq) / 4);
    }

    /* Compute a(n) = T(i,j) = T0j * 3^i + (3^i - 1)/2 */
    seq_t result = T0j * pow3 + ((pow3 - 1) / 2);
    return result;
}
