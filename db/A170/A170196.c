/*
 * A170196  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:   a(n) = 41*(a(n-1)+…+a(n-38)) – 861*a(n-39) + p_n
   where p_n = 1 for n = 0 or 39, p_n = 2 for 1 ≤ n ≤ 38, and p_n = 0 otherwise.
   Initial condition: a(0) = 1.  (The first 39 terms can also be obtained from
   the recurrence with the natural “zero‑extension’’ of a(k)=0 for k<0.) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Circular buffer holding the last 39 computed values. */
    seq_t buf[39] = {0};
    buf[0] = (seq_t)1;            /* a(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t p = 0;
        if (i == 39)          p = (seq_t)1;
        else if (i <= 38)     p = (seq_t)2;
        /* sum of at most the previous 38 terms */
        seq_t sum38 = 0;
        for (idx_t j = 1; j <= 38 && i >= j; ++j)
            sum38 += buf[(i - j) % 39];

        seq_t a_i = 41 * sum38;
        if (i >= 39) a_i -= 861 * buf[(i - 39) % 39];
        a_i += p;

        buf[i % 39] = a_i;
    }

    return buf[n % 39];
}
