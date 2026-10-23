/*
 * A024463  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (Fibonacci numbers), t = (odd natural numbers).
 * https://oeis.org/A024463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} F_i * (2·(n+1-i) – 1) where F_i are
 *            Fibonacci numbers (F_1 = 1, F_2 = 1) and the odd numbers are 1,
 *            3, 5, …
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = floor((n+1)/2) */
    idx_t k = (n + 1) >> 1;

    seq_t sum   = 0;
    seq_t F_prev = 1;  /* F_1 */
    seq_t F_curr = 1;  /* F_2 */

    for (idx_t i = 1; i <= k; ++i) {
        /* compute the i‑th summand */
        idx_t m   = n + 1 - i;                     /* 1 ≤ m ≤ n */
        seq_t odd = ((seq_t)2 * (seq_t)m) - 1;     /* 2m‑1 */
        sum += F_prev * odd;                       /* F_i * odd */

        /* advance Fibonacci numbers */
        if (i < k) {
            seq_t next = F_prev + F_curr;          /* F_{i+1} = F_i + F_{i-1} */
            F_prev = F_curr;
            F_curr = next;
        }
    }

    return sum;
}
