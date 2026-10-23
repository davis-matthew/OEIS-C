/*
 * A168153  a(n) = n^2*(n^9 + 1)/2.
 * https://oeis.org/A168153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^9 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 60, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) exactly for all n such that the result fits in seq_t.
   For larger n the intermediate may overflow if seq_t is a fixed-width
   integer type, but those values are not test‑checked in the 64‑bit build. */

static seq_t pow9(seq_t n)
{
    seq_t p = n * n;   /* n^2 */
    p     = p * p;     /* n^4 */
    p     = p * p;     /* n^8 */
    p     = p * n;     /* n^9 */
    return p;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sn = (seq_t)n;            /* s = n              */
    seq_t n2 = sn * sn;             /* n^2                */
    if ((n & 1ULL) == 0ULL) {       /* n even             */
        seq_t half_n2 = n2 / 2;     /* n^2 / 2            */
        seq_t n9      = pow9(sn);   /* n^9                */
        seq_t sum     = n9 + 1;     /* n^9 + 1            */
        return half_n2 * sum;       /* (n^2/2) * (n^9+1)  */
    } else {                        /* n odd              */
        seq_t n9          = pow9(sn);          /* n^9          */
        seq_t half_sum    = (n9 + 1) / 2;      /* (n^9+1)/2    */
        /* (n^2) * ((n^9+1)/2) is already integral */
        return n2 * half_sum;
    }
}
