/*
 * A176850  a(n,k) is the number of ways to choose integers i,j from
 *          {0,1,...,k} such that the inequalities |i-j| <= n <= i+j are
 *          satisfied.
 * https://oeis.org/A176850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 9767, u32 n <= 9767, u64 n <= 9767,
 *            u128 n <= 9767, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*===============================================================*/
/*  A176850 – number of ways to choose i,j in {0,…,k} that satisfy*/
/*  |i-j| ≤ n ≤ i+j.  The sequence is indexed row‑wise with rows*/
/*  of lengths 1,3,5,7,… .  For a given overall index n we first   */
/*  find k = ⌊√n⌋ and the offset c = n – k² (0 ≤ c ≤ 2k).  Then  */
/*  the term is                                                           */
/*                                                                          */
/*  If 0 ≤ c ≤ k :                                                    */
/*      a = ( -3c² + 4kc + c + 2k + 2 ) / 2                           */
/*                                                                          */
/*  Else (k < c ≤ 2k) :                                               */
/*      t = 2k - c                                                     */
/*      a = (t+1)(t+2)/2                                               */
/*  Where k and c are unsigned integers; arithmetic is done in seq_t.   */
/*  (seq_t is an integer type supplied by the build; idx_t is uint64_t.)   */
/*=======================================================================*/

static idx_t isqrt_u64(idx_t n)
{
    /* Largest s.t. x*x <= n */
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid != 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Find row index k and column offset c */
    idx_t k  = isqrt_u64(n);          /* 0 ≤ k ≤ √n */
    idx_t k2 = k * k;                 /* k² as idx_t (fits in uint64_t) */
    idx_t c  = n - k2;                /* 0 ≤ c ≤ 2k */

    seq_t ks = (seq_t)k;              /* convert to seq_t for arithmetic */
    seq_t cs = (seq_t)c;

    if (c <= k) {                     /* first half of the row */
        seq_t left  = 4 * ks * cs + cs + 2 * ks + 2;   /* 4kc + c + 2k + 2 */
        seq_t right = 3 * cs * cs;                     /* 3c²          */
        return (left - right) / 2;
    } else {                          /* second half of the row */
        seq_t t = (seq_t)(2 * k - c);   /* 2k - c, fits in idx_t */
        return (t + 1) * (t + 2) / 2;
    }
}
