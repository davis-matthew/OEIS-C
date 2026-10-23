/*
 * A170124  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: let p_n = 1 if n = 0 or n = 38, 2 if 1 ≤ n ≤
 *            37, 0 otherwise; a(n) = p_n + 17·(a(n‑1) + a(n‑2) + … + a(n‑37))
 *            – (n ≥ 38 ? 153·a(n‑38) : 0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ring buffer holding the last 38 terms a(i‑38) … a(i‑1)  */
    seq_t buf[38];
    seq_t sumPrev = 0;           /* sum of the last up to 37 terms */

    for (idx_t i = 0; i <= n; ++i) {
        /* Determine p_n for the current index n = i */
        seq_t p;
        if (i == 0 || i == 38)
            p = (seq_t)1;
        else if (i <= 37)
            p = (seq_t)2;
        else
            p = (seq_t)0;

        /* Compute a(i) using the recurrence */
        seq_t val = p + (seq_t)17 * sumPrev;
        if (i >= 38) {
            seq_t old = buf[i % 38];                /* a(i‑38) before overwrite */
            val -= (seq_t)153 * old;
        }
        buf[i % 38] = val;                              /* store a(i) */

        /* Update sumPrev for the next iteration:
           sum of a(i) … a(i‑36)  (the next 37 previous terms)   */
        if (i >= 37) {
            seq_t old_remove = buf[(i - 37) % 38];     /* a(i‑37) which will be removed */
            sumPrev += val - old_remove;
        } else {
            sumPrev += val;
        }
    }
    return buf[n % 38];
}
