/*
 * A168797  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function:
   Let a(0)=1. For n≥1 define
         p_n = 2           if 1≤n≤18,
         p_n = 1           if n=0 or n=19,
         p_n = 0           otherwise.
   Then
         a(n) = 22·∑_{k=1}^{min(18,n)} a(n−k) − 253·a(n−19) + p_n .
   This recurrence reproduces A168797. */
seq_t a(idx_t n)
{
    /* Direct case for the first term. */
    if (n == 0U)
        return (seq_t)1;

    /* Small fixed ring buffer that holds the last 19 computed terms. */
    seq_t buf[19] = {0};
    /* buf[0] holds a(0) to start with. */
    buf[0] = (seq_t)1;
    /* sum18 stores Σ_{i=1}^{min(18,current)} a(current-i),
       i.e. the sum of the last up to 18 previous terms. */
    seq_t sum18 = buf[0];

    for (idx_t i = 1U; i <= n; ++i) {
        /* p_n for this index. */
        seq_t p = (i <= 18U) ? (seq_t)2 :
                  (i == 19U ? (seq_t)1 : (seq_t)0);

        /* Compute a(i) using the recurrence. */
        seq_t val = p + (seq_t)22 * sum18;
        if (i >= 19U)
            val -= (seq_t)253 * buf[(i - 19U) % 19U];

        /* Store the newly computed term in the circular buffer. */
        buf[i % 19U] = val;

        /* Update sum18 for the next iteration. */
        if (i < 18U) {
            sum18 += val;                      /* window still <18 */
        } else {
            /* Window size is now 18 – drop the term that slides out. */
            sum18 = sum18 + val - buf[(i - 18U) % 19U];
        }
    }

    /* Result resides at index n mod 19. */
    return buf[n % 19U];
}
