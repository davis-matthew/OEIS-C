/*
 * A024384  a(n) = s(1)*s(2)*...*s(n+1)*(1/s(2) - 1/s(3) + ... + c/s(n+1)),
 *          where c = (-1)^(n+1) and s(k) = 4k-3 for k = 1,2,3,...
 * https://oeis.org/A024384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(1)*s(2)*...*s(n+1) * * (1/s(2) - 1/s(3) + 1/s(4) - … +
 *            (-1)^(n+1)/s(n+1)), * where s(k) = 4k-3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 to indicate that. */
    if (n < 1) return (seq_t)0;

    seq_t p = (seq_t)1;      /* P_k = product s(1)…s(k) */
    seq_t a_prev = (seq_t)1; /* a(1) = 1 */

    /* Loop from k = 2 to n; at each step compute a(k). */
    for (idx_t k = 2; k <= n; ++k) {
        /* Update product to P_k = P_{k-1} * s(k). */
        seq_t s_k   = (seq_t)(4 * k - 3); /* s(k) = 4k - 3 */
        p = p * s_k;                      /* now p = P_k */

        /* s(k+1) needed for the recurrence. */
        seq_t s_k1  = (seq_t)(4 * k + 1);  /* s(k+1) = 4(k+1)-3 = 4k+1 */

        /* Compute a(k) = a(k-1)*s(k+1) + (-1)^(k+1) * P_k. */
        seq_t term = a_prev * s_k1;
        if ((k & 1) == 0) {      /* k even → (-1)^(k+1) = -1 */
            term = term - p;     /* subtraction is safe: result > 0 */
        } else {                  /* k odd → (+1) */
            term = term + p;
        }
        a_prev = term;
    }
    return a_prev;
}
