/*
 * A156622  Values of register a when register b becomes 0 for the
 *          two-register machine {i[1], i[1], i[1], d[2,1], d[1,6], i[2],
 *          d[1,5], d[2,3]}.
 * https://oeis.org/A156622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3*A156623(n+1) – 2, where A156623(2)=1, A156623(3)=2, and
 *            for m ≥ 3, A156623(m+1) = ceil(3*A156623(m)/2)
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* We need A156623(k) with k = n+1.  The recurrence is
       A156623(k+1) = (3*A156623(k) + 1) / 2  for k ≥ 2
       (since ceil(3*x/2) = (3*x + 1)/2 for non‑negative x). */

    if (n == 0)
        return (seq_t)0;             /* Undefined for offset 1, but harmless */

    idx_t k = n + 1;                 /* Index for A156623 */
    seq_t B;

    if (k <= 2)                      /* A156623(2) = 1 */
    {
        B = (seq_t)1;
    }
    else if (k == 3)                 /* A156623(3) = 2 */
    {
        B = (seq_t)2;
    }
    else
    {
        seq_t b_prev = (seq_t)1;     /* A156623(2) */
        seq_t b_curr = (seq_t)2;     /* A156623(3) */

        for (idx_t i = 4; i <= k; ++i)
        {
            /* Ceil(3 * b_curr / 2) = (3 * b_curr + 1) / 2 */
            seq_t b_next = ((seq_t)3 * b_curr + (seq_t)1) / (seq_t)2;
            b_prev = b_curr;
            b_curr = b_next;
        }
        B = b_curr;
    }

    return ((seq_t)3 * B) - (seq_t)2;
}
