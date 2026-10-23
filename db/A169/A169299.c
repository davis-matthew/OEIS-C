/*
 * A169299  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 46 for 2 ≤ n ≤ 28: a(n) = 45 * a(n-1) for n ≥
 *            29: a(n) = 990 * a(n-29) − 44 * (a(n-1) + a(n-2) + … + a(n-28))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first term special */
    if (n == 0ULL)
        return (seq_t)1;

    /* Compute the initial 29 terms (0 … 28) */
    if (n <= 28ULL) {
        seq_t val = (seq_t)46;          /* a(1) */
        if (n == 1ULL)
            return val;
        for (idx_t i = 2; i <= n; ++i)
            val = (seq_t)45 * val;     /* a(i) = 45 * a(i-1) */
        return val;
    }

    /* For n ≥ 29 we use a linear recurrence of order 29.
       The following circular buffer holds the last 29 values
       (buf[0] = a(k-29), buf[28] = a(k-1)). */
    seq_t buf[29];
    buf[0] = (seq_t)1;           /* a(0) */
    buf[1] = (seq_t)46;          /* a(1) */
    for (idx_t i = 2; i <= 28; ++i)
        buf[i] = (seq_t)45 * buf[i-1];   /* a(i) for 2…28 */

    /* Iterate from 29 up to n */
    for (idx_t cur = 29; cur <= n; ++cur) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= 28; ++j)
            sum += buf[j];                /* Σ a(cur-j) for j=1..28 */

        seq_t val = (seq_t)990 * buf[0] - (seq_t)44 * sum;  /* a(cur) */

        /* Shift buffer left, appending the new value at the end */
        for (idx_t k = 0; k < 28; ++k)
            buf[k] = buf[k + 1];
        buf[28] = val;
    }

    return buf[28];
}
