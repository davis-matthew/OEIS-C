/*
 * A237655  G.f.: exp( Sum_{n>=1} 5*Fibonacci(n-2)*Fibonacci(n+2) * x^n/n ).
 * https://oeis.org/A237655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: (1+x)^7 / (1-3*x+x^2) * Recurrence: a(n) = 3*a(n-1) -
 *            a(n-2) for n >= 8 * Initial values: a(0)=1, a(1)=10, a(2)=50,
 *            a(3)=175, * a(4)=510, a(5)=1376, a(6)=3625, a(7)=9500
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fixed initial values for n = 0 … 7 */
    static const seq_t init[8] = {
        1, 10, 50, 175, 510, 1376, 3625, 9500
    };

    /* handle the offset range directly */
    if (n < 8)
        return init[n];

    /* start recurrence from a(6) and a(7) */
    seq_t a_nm2 = init[6];   /* a(n-2) */
    seq_t a_nm1 = init[7];   /* a(n-1) */
    seq_t a_n   = a_nm1;     /* placeholder for a(n) */

    /* iterate up to desired n */
    for (idx_t i = 8; i <= n; ++i)
    {
        a_n = a_nm1 * 3 - a_nm2;  /* a(n) = 3*a(n-1)-a(n-2) */
        a_nm2 = a_nm1;            /* shift indices for next step */
        a_nm1 = a_n;
    }

    return a_n;
}
