/*
 * A069007  Let M denote the 6 X 6 matrix with rows
 *          /1,1,1,1,1,1/1,1,1,1,1,0/1,1,1,1,0,0/1,1,1,0,0,0/1,1,0,0,0,0/1,0,0,0,0,0/
 *          and A(n) the vector (x(n),y(n),z(n),t(n),u(n),v(n)) = M^n*A where
 *          A is the vector (1,1,1,1,1,1); then a(n) = y(n).
 * https://oeis.org/A069007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 3*a(n-1) +
 *            6*a(n-2) - 4*a(n-3) - 5*a(n-4) + a(n-5) + a(n-6) * with initial
 *            values * a(0) = 1, a(1) = 5, a(2) = 20, a(3) = 85, a(4) = 350,
 *            a(5) = 1456. * This recurrence comes from the generating
 *            function * (x^3+x^2-2*x-1) / (x^6+x^5-5*x^4-4*x^3+6*x^2+...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return pre‑computed initial values for n < 6 */
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 20;
    if (n == 3) return 85;
    if (n == 4) return 350;
    if (n == 5) return 1456;

    /* Iterate the recurrence up to n. */
    seq_t a0 = 1,  /* a(n-6)   */
          a1 = 5,  /* a(n-5)   */
          a2 = 20, /* a(n-4)   */
          a3 = 85, /* a(n-3)   */
          a4 = 350,/* a(n-2)   */
          a5 = 1456;/* a(n-1)  */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 3 * a5                      /* 3*a(n-1) */
                   + 6 * a4                      /* +6*a(n-2) */
                   - 4 * a3                      /* -4*a(n-3) */
                   - 5 * a2;                     /* -5*a(n-4) */
        next = next + a1 + a0;                    /* +a(n-5)+a(n-6) */

        /* Shift window */
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = next;
    }

    return a5;  /* a(n) */
}
