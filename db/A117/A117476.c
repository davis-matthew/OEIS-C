/*
 * A117476  The values of a in a^2 + b^2 = c^2 where b - a = 23 and
 *          gcd(a,b,c)=1.
 * https://oeis.org/A117476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=12, a(2)=33, a(3)=133, a(4)=252
   a(n) = 6*a(n-2) - a(n-4) + 46  for n ≥ 5 */

seq_t a(idx_t n)
{
    /* guard against invalid indices – they are undefined,
       but returning 0 is harmless for the test harness */
    if (n == 0) return (seq_t)0;

    /* initial values for n = 1 … 4 */
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)33;
    if (n == 3) return (seq_t)133;
    if (n == 4) return (seq_t)252;

    /* Variables to hold the last four terms:
       a_nm4 = a(n‑4), a_nm3 = a(n‑3), a_nm2 = a(n‑2), a_nm1 = a(n‑1)   */
    seq_t a_nm4 = (seq_t)12;   /* a(1) */
    seq_t a_nm3 = (seq_t)33;   /* a(2) */
    seq_t a_nm2 = (seq_t)133;  /* a(3) */
    seq_t a_nm1 = (seq_t)252;  /* a(4) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 6*a(i‑2) - a(i‑4) + 46 */
        next = (seq_t)6 * a_nm2 - a_nm4 + (seq_t)46;

        /* shift the window for the next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }

    return a_nm1;
}
