/*
 * A204175  Symmetric matrix based on f(i,j)=(1 if max(i,j) is even, and 0
 *          otherwise), by antidiagonals.
 * https://oeis.org/A204175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A204175:  a(n) = 1 if the maximum of the coordinates (i,j) of the
 *  n‑th entry read by antidiagonals in the infinite matrix
 *  M(i,j) = 1 when max(i,j) is even, otherwise 0; otherwise 0.  */
seq_t a(idx_t n)
{
    if (n == 0u)               /* offset is 1 – undefined for n=0 */
        return (seq_t)0;

    idx_t sum = 0;          /* cumulative count of elements up to current diagonal */
    idx_t s   = 2;          /* current diagonal sum i+j */

    /* find the diagonal that contains the n‑th element */
    for (;;)
    {
        sum += s - 1;      /* each diagonal has (s-1) elements */
        if (sum >= n)
            break;
        ++s;
    }

    /* offset within this diagonal (0‑based) */
    idx_t prev_sum = sum - (s - 1);
    idx_t offset   = n - prev_sum - 1;

    /* coordinates of the element */
    idx_t i = offset + 1;
    idx_t j = s - i;

    /* evenness of the larger coordinate */
    idx_t mx = (i > j) ? i : j;
    return (seq_t)((mx & 1) ^ 1);  /* 1 if even, 0 if odd */
}
