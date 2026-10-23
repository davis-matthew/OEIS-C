/*
 * A133673  a(n) = n*L(n) + (n-1)*L(n-1) where L(n) is the Lucas number.
 * https://oeis.org/A133673
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*L(n) + (n-1)*L(n-1) where L(n) is the Lucas number *
 *            defined by L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 37, u64 n <= 82, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(2) is the first defined term; values for n<2 are unused and
       returned as 0. */
    if (n < 2)
        return 0;

    /* Initial Lucas numbers: L(1)=1, L(2)=3. */
    seq_t L_prev = 1; /* L(n-1) for n=2 */
    seq_t L_curr = 3; /* L(n)   for n=2 */

    /* Compute Lucas numbers up to L(n). */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t L_next = L_curr + L_prev;
        L_prev = L_curr;
        L_curr = L_next;
    }

    /* Compute a(n) = n*L(n) + (n-1)*L(n-1). */
    seq_t result = (seq_t)n * L_curr + (seq_t)(n - 1) * L_prev;
    return result;
}
