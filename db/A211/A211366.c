/*
 * A211366  Array read by antidiagonals: T(m,n) = Sum(1<=i<=m) i * ( n +
 *          2(i-1) )!
 * https://oeis.org/A211366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{m=1}^{t} m * (r + 2*(m-1))! where t = the minimal
 *            integer with t*(t+1)/2 ≥ n, and r = t + 1 – m. This is the array
 *            read by antidiagonals of T(m,n) = Σ_{i=1..m} i·(n + 2*(i-1))!
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 64, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Example values: a(1)=1, a(2)=2, a(3)=13, a(4)=6, a(5)=50, a(6)=373, … */

/* The function assumes that the integer types `idx_t` (unsigned)
 * and `seq_t` (an unsigned integer type or arbitrary‑precision class)
 * are defined by the build system. */

seq_t a(idx_t n)
{
    /* Find the diagonal t such that Σ_{k=1}^t k ≥ n. */
    idx_t t  = 1;
    idx_t cum = 1;          /* cumulative count up to current t */
    while (cum < n) {
        ++t;
        cum += t;
    }

    /* Count of elements before this diagonal. */
    idx_t pre = (t - 1) * t / 2;

    /* Position inside the diagonal (1‑based). */
    idx_t k = n - pre;          /* 1 ≤ k ≤ t */
    idx_t m = k;                /* column index (m)   */
    idx_t r = t + 1 - m;        /* row index (r)      */

    /* Compute T(m,r) = Σ_{i=1..m} i * (r + 2*(i-1))! . */
    seq_t sum = 0;
    seq_t fact = 1;   /* will hold factorial of current (r+2*(i-1)). */
    idx_t arg   = r;  /* current argument of the factorial. */

    /* factorial(r) */
    for (idx_t i = 1; i <= arg; ++i)
        fact *= (seq_t)i;

    /* i = 1 term */
    sum += fact;  /* term is 1 * factorial(r) */

    /* iterate i = 2 .. m */
    for (idx_t i = 2; i <= m; ++i) {
        /* factorial(arg + 2) = factorial(arg) * (arg+1)*(arg+2) */
        fact *= (seq_t)(arg + 1);
        fact *= (seq_t)(arg + 2);
        arg += 2;
        sum += fact * (seq_t)i;
    }

    return sum;
}
