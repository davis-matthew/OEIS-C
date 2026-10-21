/*
 * A216248  T(n,k)=((n+k)^2-4*k+3-(-1)^k-(n+k-2)*(-1)^(n+k))/2-1, if k=1 and
 *          (n mod 2)=1; T(n,k)=((n+k)^2-4*k+3-(-1)^k-(n+k-2)*(-1)^(n+k))/2,
 *          else. Table T(n,k) read by antidiagonals; n, k > 0.
 * https://oeis.org/A216248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where * T(n,k) = ((n+k)^2 - 4*k + 3 - (-1)^k -
 *            (n+k-2)*(-1)^(n+k))/2 * minus 1 if k = 1 and n odd. The index n
 *            is interpreted as a * position in the antidiagonal enumeration
 *            of the T table
 * Range:     u8 n <= 242, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* locate the diagonal t containing position n */
    idx_t t = 2;
    while (((t - 1) * t) / 2 < n) {
        ++t;
    }

    /* sum of entries before diagonal t */
    idx_t prev_sum = ((t - 2) * (t - 1)) / 2;
    idx_t offset   = n - prev_sum - 1;       /* 0‑based offset in diagonal t */

    /* convert offset to row (i) and column (j)   */
    idx_t i = offset + 1;                    /* row index  */
    idx_t j = (t - 1) - offset;              /* column index */

    /* compute T(i,j) */
    seq_t ni = (seq_t)i;
    seq_t kj = (seq_t)j;

    seq_t sum   = ni + kj;                   /* n + k            */
    seq_t term  = sum * sum;                 /* (n+k)^2          */
    term -= (seq_t)4 * kj;                   /* -4*k             */
    term += (seq_t)3;                        /* +3               */

    /* adjust for (-1)^j   */
    if (j % 2 == 0) {                        /* (-1)^j = 1 */
        term -= (seq_t)1;                    /* subtract 1      */
    } else {                                 /* (-1)^j = -1 */
        term += (seq_t)1;                    /* subtract -1 -> +1 */
    }

    /* adjust for (n+k-2)*(-1)^(n+k)   */
    seq_t rem = sum - (seq_t)2;               /* n + k - 2       */
    if (((i + j) % 2) == 0) {                /* (-1)^(n+k) = 1 */
        term -= rem;                         /* subtract (n+k-2)*/
    } else {                                 /* (-1)^(n+k) = -1*/
        term += rem;                         /* subtract -(n+k-2) = + (n+k-2) */
    }

    term /= (seq_t)2;  /* divide by 2 */

    /* final minus 1 if k==1 and i odd  */
    if ((j == 1) && (i % 2 == 1)) {
        term -= (seq_t)1;
    }

    return term;
}
