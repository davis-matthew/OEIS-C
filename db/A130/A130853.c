/*
 * A130853  Runs of 1's of lengths 1, Fibonacci numbers F(1), F(2), F(3), ...
 *          (A000045) separated by 0's.
 * https://oeis.org/A130853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9825, u16 n <= 9825, u32 n <= 9825, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth bit (1‑indexed) of the binary string that starts with
   0 and, for each k ≥ 1, appends F(k) 1's followed by 0, where
   F(k) is the k‑th Fibonacci number (F(1)=1, F(2)=1, …).
   For n > 1 find the smallest i such that
     1 + Σ_{k=1..i} (F(k)+1) ≥ n.
   Let
     t = n-1-Σ_{k=1..i-1} (F(k)+1).
   Then a(n) = 1 if t < F(i), else a(n)=0. */
seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* first element is the leading 0 */

    /* cumulative length of the string seen so far (1 = the first 0) */
    idx_t cum = 1;
    /* Fibonacci numbers:  F(0)=0, F(1)=1 */
    seq_t f_prev = 0;
    seq_t f_curr = 1;              /* this will be F(i) in the loop */

    for (;;)
    {
        /* length of the current block = F(i) 1's + a trailing 0 */
        seq_t block_len_seq = f_curr + 1;
        idx_t block_len = (idx_t)block_len_seq;

        if (cum + block_len >= n)
        {
            /* position of n inside the current block (0‑based) */
            idx_t offset = n - cum - 1;
            if (offset < (idx_t)f_curr)
                return 1;          /* inside the run of 1's */
            else
                return 0;          /* the trailing 0 */
        }

        cum += block_len;          /* move past this block */

        /* advance Fibonacci numbers: F(i+1) = F(i-1) + F(i) */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
}
