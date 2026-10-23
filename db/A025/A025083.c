/*
 * A025083  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (Fibonacci numbers), t = (odd natural numbers).
 * https://oeis.org/A025083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} F_i · (2·(n-i+1)+1) * where F_i is
 *            the i-th Fibonacci number (F_1=1, F_2=1, …), * and the odd
 *            natural numbers are 3,5,7,… (t(m)=2m+1)
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The build supplies seq_t and idx_t.  No other declarations are needed. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                      /* undefined for n=0, return 0 */

    /* k = ⌊(n+1)/2⌋  (integer division yields the floor) */
    idx_t k = (n + 1) / 2;

    seq_t sumF    = (seq_t)0;                 /* Σ F_i    for i=1..k  */
    seq_t sumIF   = (seq_t)0;                 /* Σ i·F_i  for i=1..k  */

    /* Compute Fibonacci numbers iteratively, starting with F1=1, F2=1 */
    seq_t prev = (seq_t)0;                    /* F0  */
    seq_t curr = (seq_t)1;                    /* F1  */

    for (idx_t i = 1; i <= k; ++i) {
        seq_t Fi = curr;                      /* F_i   */
        sumF   += Fi;
        sumIF  += (seq_t)i * Fi;

        if (i == k) break;                   /* last term computed */
        seq_t next = prev + curr;            /* F_{i+1} = F_{i-1} + F_i */
        prev = curr;
        curr = next;
    }

    seq_t N   = (seq_t)n;
    seq_t res = ((seq_t)2 * N + (seq_t)3) * sumF - (seq_t)2 * sumIF;
    return res;
}
