/*
 * A135072  Minimal values of m associated with A135061.
 * https://oeis.org/A135072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + floor( ( t – sqrt(t² – 4·n³) ) / 2 ) where t = floor(
 *            2·sqrt( n³ ) ) + 1
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 31, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal integer square‑root routine for seq_t.
   It works for any positive seq_t that supports arithmetic operators. */
static seq_t isqrt(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    while (lo < hi)
    {
        /* Midpoint calculation without overflow: (lo + hi + 1) / 2 */
        seq_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1. */
    seq_t sn      = (seq_t)n;          /* cast index to seq_t */
    seq_t n_sq    = sn * sn;           /* n²           */
    seq_t n_cu    = n_sq * sn;         /* n³           */
    seq_t four_ncu = n_cu << 2;        /* 4 · n³       */
    seq_t sqrt4ncu = isqrt(four_ncu);  /* floor( sqrt(4·n³) ) = floor(2·sqrt(n³)) */
    seq_t t = sqrt4ncu + 1;            /* t = floor(2·sqrt(n³)) + 1 */

    seq_t t_sq   = t * t;              /* t²            */
    seq_t diff   = t_sq - four_ncu;    /* D = t² – 4·n³  */
    seq_t s      = isqrt(diff);        /* floor( sqrt(D) ) */
    seq_t k      = t - s;              /* k = t – s     */
    seq_t m      = k >> 1;             /* floor(k/2)    */

    seq_t val;
    /* Check whether D is a perfect square */
    if (s * s == diff)
        val = m;                       /* perfect square => floor((t‑s)/2) = m */
    else
    {
        /* D not a perfect square.
           If k is odd, floor((k‑δ)/2) = m.
           If k is even, floor((k‑δ)/2) = m‑1. */
        if (k & 1)
            val = m;
        else
            val = m - 1;
    }

    return val + 1;                    /* final formula adds 1 */
}
