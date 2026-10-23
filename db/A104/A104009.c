/*
 * A104009  Area of (a,a,a+1)-integer triangle. Corresponding a's are in
 *          A103974.
 * https://oeis.org/A104009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence formula for A104009 (side‑length a of an (a,a,a+1) triangle):
 *
 * S0 = 2, S1 = 14  (S_n =  (2+√3)^(2n) + (2-√3)^(2n))
 * D0 = 0, D1 = 4  (D_n = (2+√3)^(2n) - (2-√3)^(2n)) / (2√3)
 *
 * For n ≥ 1:
 *    S_{i+1} = 14 * S_i - S_{i-1}
 *    D_{i+1} = 14 * D_i - D_{i-1}
 *    a_{i+1} = (D_i * (S_i + 4)) / 6
 *
 * a(1)=0, a(2)=12, a(3)=1848, …
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;                 /* a(1) */
    seq_t S_prev = 2;          /* S0 */
    seq_t S_curr = 14;         /* S1 */
    seq_t D_prev = 0;          /* D0 */
    seq_t D_curr = 4;          /* D1 */
    seq_t val = 0;

    for (idx_t i = 1; i < n; ++i) {
        val = (D_curr * (S_curr + 4)) / 6;
        if (i == n - 1) break;          /* computed a(n) */

        seq_t S_next = 14 * S_curr - S_prev;
        seq_t D_next = 14 * D_curr - D_prev;

        S_prev = S_curr;
        S_curr = S_next;
        D_prev = D_curr;
        D_curr = D_next;
    }
    return val;
}
