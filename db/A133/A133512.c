/*
 * A133512  Accept F(1), reject F(1), accept F(2), reject F(2), accept F(3),
 *          ...,.
 * https://oeis.org/A133512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n‑1) + F_k, where F_k is the Fibonacci number such that
 *            F_k – 1 ≤ (n – 1) ≤ F_k + F_{k‑1} – 2
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */
    idx_t r = n - 1;                /* block length that must repeat */

    /*  Fibonacci numbers, starting with F1 = 1, F2 = 1 */
    idx_t f_prev = 1;   /* F1 */
    idx_t f_curr = 1;   /* F2 */

    while (1) {
        idx_t start = f_curr - 1;
        idx_t end   = f_curr + f_prev - 2;
        if (r >= start && r <= end) {
            return (seq_t)r + (seq_t)f_curr;
        }
        /* advance to the next Fibonacci number */
        idx_t f_next = f_curr + f_prev;
        f_prev = f_curr;
        f_curr = f_next;
    }
}
