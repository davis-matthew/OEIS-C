/*
 * A034116  Fractional part of cube root of a(n) starts with digit 0 (cubes
 *          excluded).
 * https://oeis.org/A034116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, look
 * Formula:   a(n) is the n-th integer such that it is not a perfect cube *
 *            and the fractional part of its cube root starts with the digit
 *            0. * For each positive integer k one obtains a block of values *
 *            k^3+1, k^3+2, ..., floor((k+0.1)^3) * (cubes themselves are
 *            excluded). The sequence is the concatenation *...
 * Range:     u8 none, u16 n <= 7, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based */
    idx_t   k   = 1;          /* current block counter (k≥1) */
    seq_t   cum = 0;          /* cumulative count of terms seen so far */
    seq_t   n_seq = (seq_t)n; /* n promoted to seq_t for comparison */

    for (;; ++k)
    {
        /* k as seq_t for arithmetic */
        seq_t kseq = (seq_t)k;

        /* k^3  (in seq_t) */
        seq_t ksq = kseq * kseq * kseq;

        /* (k + 0.1)^3 = ((10k + 1)/10)^3  ->  ((10k+1)^3) / 1000 */
        seq_t base    = (seq_t)10 * kseq + 1;      /* 10k + 1        */
        seq_t cube10  = base * base * base;       /* (10k+1)^3      */
        seq_t T       = cube10 / 1000;            /* floor((k+0.1)^3) */

        seq_t L = T - ksq;        /* number of terms in this block */

        if (L > (seq_t)0)
        {
            if (cum + L >= n_seq)        /* the desired term lies here */
            {
                seq_t  offset_seq = n_seq - cum; /* 1‑based offset in block */
                idx_t  offset     = (idx_t)offset_seq;   /* safe, L small */
                return ksq + (seq_t)offset;
            }
            cum += L;                     /* move to next block */
        }
        /* else L==0 (for k=1): nothing to add, just continue */
    }
}
