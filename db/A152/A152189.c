/*
 * A152189  Product_{k=1..floor((n-1)/2)} (1 + 4*cos(k*Pi/n)^2)*(1 +
 *          4*sin(k*Pi/n)^2).
 * https://oeis.org/A152189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..floor((n-1)/2)}
 *            (1+4*cos(k*Pi/n)^2)*(1+4*sin(k*Pi/n)^2) /* Equivalent simplified
 *            recurrence: * a(0) = 1 * a(n) = F(2n) if n is odd * a(n) =
 *            F(n)^2 if n is even * where F(m) is the mth Fibonacci number
 *            (F(0)=0, F(1)=1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;          /* a(0) = 1 */

    /* Compute Fibonacci numbers up to the needed index.
     * For odd n we need F(2n); for even n we need F(n). */
    idx_t target = (n & 1) ? (n * (idx_t)2) : n;

    /* Fibonacci recurrence: F(0)=0, F(1)=1 */
    seq_t f_prev = (seq_t)0;  /* F(i-2) */
    seq_t f_curr = (seq_t)1;  /* F(i-1) */
    seq_t Fn = (seq_t)0;      /* F(n), needed only for even n */

    /* Special case: n==1 => Fn set to F(1) */
    if (n == (idx_t)1) Fn = (seq_t)1;

    for (idx_t i = (idx_t)2; i <= target; ++i) {
        seq_t f_next = f_prev + f_curr; /* F(i) = F(i-1)+F(i-2) */
        f_prev = f_curr;
        f_curr = f_next;
        if (i == n) Fn = f_curr;      /* Capture F(n) if we reach it */
    }

    if ((n & 1) == (idx_t)0) {
        /* Even n: a(n) = F(n)^2 */
        return Fn * Fn;
    } else {
        /* Odd n: a(n) = F(2n) */
        return f_curr;
    }
}
