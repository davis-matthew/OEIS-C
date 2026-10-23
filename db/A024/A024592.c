/*
 * A024592  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (F(2), F(3), ...), t = (odd natural numbers).
 * https://oeis.org/A024592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{k} F(i+1) * (2*(n+1-i)-1), where k = ⌊(n+1)/2⌋,
 *            F(0)=0, F(1)=1 (the Fibonacci sequence)
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial offset; n >= 1 as per OEIS. */
    if (n == 0) return (seq_t)0;

    /* k = floor((n+1)/2); for integer division this is just (n+1)/2. */
    idx_t k = (n + 1) / 2;

    /* Fibonacci numbers F(1)=1, F(2)=1 used iteratively. */
    seq_t F_prev = (seq_t)1; /* F(1) */
    seq_t F_curr = (seq_t)1; /* F(2) */

    seq_t result = (seq_t)0;

    for (idx_t i = (idx_t)1; i <= k; ++i) {
        /* odd term: (2*(n+1-i)-1) fits in idx_t. */
        idx_t odd_idx = 2 * (n + 1 - i) - 1;
        seq_t odd = (seq_t)odd_idx;

        result += F_curr * odd;

        /* Advance to next Fibonacci number. */
        seq_t next = F_prev + F_curr;
        F_prev = F_curr;
        F_curr = next;
    }

    return result;
}
