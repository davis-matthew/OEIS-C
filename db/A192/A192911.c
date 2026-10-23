/*
 * A192911  Constant term in the reduction by (x^3 -> x + 1) of the polynomial
 *          F(n+1)*x^n, where F(n)=A000045 (Fibonacci sequence).
 * https://oeis.org/A192911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+4*a(n-2)+5*a(n-3)+2*a(n-4)-a(n-5)+a(n-6) | *
 *            initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=3, a(4)=5, a(5)=16
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial six values */
    const seq_t init[6] = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)3, (seq_t)5, (seq_t)16 };

    if (n < 6) return init[n];

    /* array holding a(n-6) .. a(n-1) */
    seq_t arr[6];
    for (int k = 0; k < 6; ++k) arr[k] = init[k];

    for (idx_t i = 6; i <= n; ++i) {
        seq_t temp = arr[5]                /* a(n-1) */
                  + (seq_t)4 * arr[4]         /* +4*a(n-2) */
                  + (seq_t)5 * arr[3]         /* +5*a(n-3) */
                  + (seq_t)2 * arr[2]         /* +2*a(n-4) */
                  + arr[0];                   /* +a(n-6) */
        seq_t next = temp - arr[1];       /* -a(n-5) */

        /* shift array left: drop a(n-6) */
        for (int m = 0; m < 5; ++m) arr[m] = arr[m + 1];
        arr[5] = next;                    /* a(n) now in arr[5] */
    }

    return arr[5];
}
