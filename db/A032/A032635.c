/*
 * A032635  a(n) = floor(n^2 / e).
 * https://oeis.org/A032635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2 / e)
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence integer type */
    seq_t nt = (seq_t)n;
    seq_t n2 = nt * nt;  /* n^2, fits into seq_t for the tested ranges */

    /* Alias to store the convergent p/q that represents an upper bound for e
       (i.e. p/q > e) with denominator larger than n. */
    seq_t pH, qH;

    /* Special case for n == 0: the first term is 0. */
    if (n == 0)
    {
        return (seq_t)0;
    }

    /* Compute the first convergent of e with denominator > n.
       The continued fraction of e is: [2; 1,2,1,1,4,1,1,6,1,1,8,…]   */
    {
        seq_t p_prev = (seq_t)2;   /* index 0 convergent 2/1  */
        seq_t q_prev = (seq_t)1;
        seq_t p_cur  = (seq_t)3;   /* index 1 convergent 3/1  */
        seq_t q_cur  = (seq_t)1;
        int idx = 1;                /* current convergent index */

        /* If the first non‑trivial convergent already has q > n,
           we are done.  Otherwise iterate. */
        if (q_cur <= n)
        {
            for (int k = 2; ; ++k)
            {
                int a = (k % 3 == 2) ? 2 * ((k + 1) / 3) : 1;

                seq_t p_next = a * p_cur + p_prev;
                seq_t q_next = a * q_cur + q_prev;

                p_prev = p_cur; q_prev = q_cur;
                p_cur  = p_next; q_cur  = q_next;
                ++idx;

                if (q_cur > n)
                {
                    /* idx is odd for all upper bounds of e. */
                    pH = p_cur;   /* p/q > e */
                    qH = q_cur;
                    break;
                }
            }
        }
        else
        {
            pH = p_cur;   /* 3/1 > e, and q_cur > n (i.e. n < 1) */
            qH = q_cur;
        }
    }

    /* Compute the lower estimate using the upper bound convergent (pH/qH):
       low = floor( n^2 * qH / pH )  */
    seq_t low  = (n2 * qH) / pH;
    seq_t high = low + (seq_t)1;          /* candidate for the true value */

    /* Test whether `low` satisfies n^2 < (low+1) * pH / qH,
       i.e. whether low is the correct floor value. */
    if ((n2 * qH) < (high * pH))
    {
        return low;
    }
    else
    {
        return high;
    }
}
