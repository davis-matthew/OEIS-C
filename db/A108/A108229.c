/*
 * A108229  n occurs Lucas number L(n) times (A000204).
 * https://oeis.org/A108229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the minimal k such that the cumulative sum of the first k
 *            Lucas numbers (L(1)=1, L(2)=3, L(i)=L(i-1)+L(i-2)) is at least n
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A108229.
   The sequence consists of k repeated L(k) times, where L(k) is the k-th Lucas number.
   The function determines the value at position n (1‑based). */
seq_t a(idx_t n)
{
    /* Lucas numbers start L(1)=1, L(2)=3 */
    seq_t lprev = 1;    /* L(1)   */
    seq_t lcur  = 3;    /* L(2)   */

    idx_t sum = 0;      /* cumulative total of L(i) */
    idx_t k   = 1;      /* current index i */

    while (1) {
        seq_t ln;          /* current Lucas number L(k) */

        if (k == 1) {
            ln = 1;       /* L(1) */
        } else if (k == 2) {
            ln = 3;       /* L(2) */
        } else {
            /* For k ≥ 3, compute next Lucas number */
            ln = lcur + lprev;   /* L(k) = L(k-1)+L(k-2) */
            lprev = lcur;        /* shift for next iteration */
            lcur  = ln;
        }

        sum += (idx_t)ln;   /* update running total */

        if (sum >= n) {
            return (seq_t)k;   /* the n-th term is k */
        }

        ++k;   /* move to next Lucas number */
    }
}
