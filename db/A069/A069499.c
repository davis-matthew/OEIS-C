/*
 * A069499  Triangular numbers of the form 21*k.
 * https://oeis.org/A069499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4*n) = 21*n*(21*n - 1)/2 Formula: a(4*n+1) = 21*n*(21*n + 1)/2
 *            Formula: a(4*n+2) = 21*(3*n + 1)*(7*n + 2)/2 Formula: a(4*n+3) =
 *            21*(3*n + 2)*(7*n + 5)/2 where n = index-1 . This computes the
 *            nth term of OEIS A069499 (triangular numbers of the form 21*k)
 *            in O(1) arithmetic time
 * Range:     u8 n <= 5, u16 n <= 69, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The input n is 1‑based: a(1)=0.  Compute m and remainder r as described
       above.  All calculations use only integral operations allowed in the
       problem statement. */
    idx_t   m = n / 4;       /* integer division (floor) */
    unsigned r = (unsigned)(n % 4);

    if (r == 0)   /* a(4*m) */
    {
        seq_t mm = (seq_t)m;
        seq_t t  = 21 * mm;           /* 21*m */
        seq_t res = mm * (t - 1);     /* m*(21m - 1) */
        res /= 2;                    /* divide by 2 */
        res *= 21;                   /* multiply by 21 */
        return res;
    }
    else if (r == 1)  /* a(4*m+1) */
    {
        seq_t mm = (seq_t)m;
        seq_t t  = 21 * mm;                /* 21*m */
        seq_t res = mm * (t + 1);          /* m*(21m + 1) */
        res /= 2;
        res *= 21;
        return res;
    }
    else if (r == 2)  /* a(4*m+2) */
    {
        seq_t mm = (seq_t)m;
        seq_t res = (3 * mm + 1) * (7 * mm + 2); /* (3m+1)(7m+2) */
        res /= 2;
        res *= 21;
        return res;
    }
    else              /* r == 3, a(4*m+3) */
    {
        seq_t mm = (seq_t)m;
        seq_t res = (3 * mm + 2) * (7 * mm + 5); /* (3m+2)(7m+5) */
        res /= 2;
        res *= 21;
        return res;
    }
}
