/*
 * A034122  Fractional part of cube root of a(n) starts with digit 6.
 * https://oeis.org/A034122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the n-th integer m whose cube root has fractional part in [0.6,0.7).
 * For each integer k = floor(cuberoot(m)) we have 
 *   ceil((k+0.6)^3) <= m <= floor((k+0.7)^3),
 *   i.e.  m in [ ceil((10k+6)^3/1000), floor((10k+7)^3/1000) ].
 * The function scans k in increasing order, subtracts the number of
 * qualifying m's in each block until the desired offset is reached.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sentinel: only defined for n>=1 */

    idx_t rem = n - 1;             /* zero‑based position in the sequence */
    idx_t k   = 0;                 /* current floor(cuberoot(m)) */

    while (1)
    {
        /* (10k+6) and (10k+7) as seq_t for later powers */
        seq_t tenk_plus6 = (seq_t)k * (seq_t)10 + (seq_t)6;
        seq_t tenk_plus7 = (seq_t)k * (seq_t)10 + (seq_t)7;

        /* (10k+6)^3 and (10k+7)^3 */
        seq_t a = tenk_plus6 * tenk_plus6 * tenk_plus6;
        seq_t b = tenk_plus7 * tenk_plus7 * tenk_plus7;

        /* block boundaries: ceil((10k+6)^3 / 1000) and floor((10k+7)^3 / 1000) */
        seq_t low  = (a + (seq_t)999) / (seq_t)1000;
        seq_t high = b / (seq_t)1000;

        if (high >= low)
        {
            seq_t cnt = high - low + (seq_t)1;   /* number of terms in this block */

            if (rem < (idx_t)cnt)
                return low + (seq_t)rem;         /* found the desired term */

            rem -= (idx_t)cnt;                  /* skip entire block */
        }

        ++k;   /* proceed to next k */
    }
}
