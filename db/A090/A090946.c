/*
 * A090946  Non-Lucas numbers: complement of A000204.
 * https://oeis.org/A090946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth non‑Lucas number (complement of Lucas numbers
 *            with L₁=1, L₂=3, Lₖ=Lₖ₋₁+Lₖ₋₂)
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates Lucas numbers on the fly and counts numbers
   that are not Lucas until it reaches the requested index. */
seq_t a(idx_t n)
{
    /* a(1) is 0 by definition */
    if (n == 1)
        return 0;

    idx_t  count = 0;   /* how many non‑Lucas numbers seen so far   */
    seq_t  cand   = 0;  /* current candidate number to test        */

    /* initial Lucas numbers: 1, 3, 4, 7, ... */
    seq_t  l_prev = 1;  /* Lₖ₋₁   */
    seq_t  l_curr = 3;  /* Lₖ     */

    while (1)
    {
        /* if candidate is not a Lucas number, count it */
        if (cand != l_prev && cand != l_curr)
        {
            ++count;
            if (count == n)
                return cand;
        }

        /* advance to next integer */
        ++cand;

        /* advance Lucas sequence until the next Lucas number
           exceeds the current candidate.  This keeps l_prev
           the largest Lucas number ≤ cand and l_curr the next one.  */
        while (l_curr <= cand)
        {
            seq_t l_next = l_prev + l_curr;  /* compute next Lucas */
            l_prev = l_curr;
            l_curr = l_next;
        }
    }
}
