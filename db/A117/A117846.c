/*
 * A117846  Given n, define a(n) as follows: let a sequence b(k) be defined by
 *          b(k+1)=b(k)+b(k)mod k; b(1)=2n-1. (Here b(k)mod k denotes the
 *          least nonnegative residue of b(k) modulo k). Let a(n) be the
 *          common value of b(k+1)-b(k) for all large k if such exists;
 *          otherwise let a(n) be 0.
 * https://oeis.org/A117846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the eventual constant difference of the sequence *
 *            defined by b(1)=2n-1 and b(k+1)=b(k)+b(k) mod k. The function *
 *            simulates this recurrence until the difference stabilises and *
 *            returns that constant. If no stabilization occurs within a *
 *            reasonable number of steps (which does not happen...
 * Range:     u8 none, u16 none, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* start value */
    seq_t b = ((seq_t)n) * 2 - 1;

    idx_t k = 1;                 /* current k (1‑based)          */
    seq_t stable_d = 0;          /* last observed difference     */
    int run = 0;                 /* consecutive repetitions of stable_d */

    /* threshold for declaring stabilization */
    const int stable_window = 2048;

    /* safety cap on iterations (enough for n <= 10000) */
    const idx_t max_iter = 200000;

    while (k <= max_iter)
    {
        seq_t d = b % (seq_t)k;   /* b(k) mod k  */
        b += d;                  /* b(k+1)      */

        if (d == stable_d)
            run++;
        else
        {
            stable_d = d;
            run = 1;
        }

        /* Once the same difference repeats many times we assume
         * the sequence has entered an arithmetic progression. */
        if (run >= stable_window)
            return stable_d;

        k++;
    }

    /* fallback: no stabilization detected */
    return 0;
}
