/*
 * A172984  For n <= 18, a(n) = Fibonacci(n) mod 5, using representatives
 *          {5,1,2,3,4} (i.e., 5 instead of the usual 0), and a(19)=2.
 * https://oeis.org/A172984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1) = a(2) = 1, for 3 ≤ n ≤ 18 a(n) = (a(n‑1)+a(n‑2)‑1) mod 5 +
 *            1, for n = 19 a(n) = (a(n‑1)+a(n‑2)‑1) mod 5 + 2. For n > 19 the
 *            same recurrence as for 3 ≤ n < 19 is used
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; behaviour below is irrelevant */

    seq_t a_prev2 = 1;             /* a(1) */
    if (n == 1) return a_prev2;

    seq_t a_prev1 = 1;             /* a(2) */
    if (n == 2) return a_prev1;

    seq_t a_curr = 0;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t sum = a_prev1 + a_prev2 - 1;   /* 1 ≤ sum ≤ 9 */
        seq_t mod = sum % (seq_t)5;          /* 0 ≤ mod ≤ 4 */
        if (i == 19)
            a_curr = mod + 2;               /* +2 for n = 19 */
        else
            a_curr = mod + 1;               /* +1 for all other n ≥ 3 */

        /* shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
