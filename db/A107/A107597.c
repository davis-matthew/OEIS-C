/*
 * A107597  Antidiagonal sums of triangle A107105: a(n) = Sum_{k=0..n}
 *          A107105(n-k,k), where A107105(n,k) = C(n,k)*(C(n,k) + 1)/2.
 * https://oeis.org/A107597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} C(n-k,k) * (C(n-k,k) + 1) / 2
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            where C(n,k) is the binomial coefficient */
seq_t a(idx_t n)
{
    seq_t sum = 0;            /* accumulated sum */
    seq_t B   = 1;            /* B = C(n,0) = 1 */
    idx_t half = n / 2;       /* k only runs to floor(n/2) */

    for (idx_t k = 0; k <= half; ++k)
    {
        /* add the term for this k */
        sum += B * (B + 1) / 2;

        if (k == half)        /* last k reached, finish */
            break;

        /* compute the next binomial B_{k+1} = C(n-(k+1), k+1) */
        /*   B_{k+1} = B_k * (n-2k)*(n-2k-1) / ((n-k)*(k+1))   */
        seq_t temp = B * (seq_t)(n - 2 * k);   /* multiply first factor */
        temp /= (seq_t)(n - k);               /* exact division */
        temp *= (seq_t)(n - 2 * k - 1);       /* multiply second factor */
        temp /= (seq_t)(k + 1);               /* exact division */
        B = temp;                            /* update for next iteration */
    }

    return sum;
}
