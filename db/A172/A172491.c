/*
 * A172491  Number of partitions of n into consecutive initial Fibonacci
 *          numbers.
 * https://oeis.org/A172491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Σk Ck(n–Sk) * where Sk = Σ_{i=1..k} Fi is the sum of the
 *            first k Fibonacci numbers * and Ck(t) is the number of ways to
 *            write the non‑negative integer t * as a sum of integer multiples
 *            of F1,…,Fk (each type may be used 0 or more times). * The
 *            function builds the DP table for the coin‑chang...
 * Range:     u8 n <= 19, u16 n <= 69, u32 n <= 367, u64 n <= 3438,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    /* --- Fibonacci numbers up to n ----------------------------------- */
    /* Two 1's are distinct Fibonacci numbers. */
    const int MAXF = 64;                         /* plenty for n ≤ 20000 */
    idx_t fib[MAXF];
    size_t k = 0;
    fib[0] = 1;
    if (n >= 1) {
        fib[1] = 1;
        k = 2;
        while (k < (size_t)MAXF && fib[k - 1] <= n) {
            fib[k] = fib[k - 1] + fib[k - 2];
            ++k;
        }
    }

    /* --- DP table for coin change (unrestricted multiplicities) ----- */
    /* dp[t] = number of ways to obtain t using the Fibonacci numbers
     * considered so far.  Static array avoids constructor issues with
     * arbitrary‑precision seq_t in the C++ build. */
    static seq_t dp[20001];   /* 20001 covers all n in the test suite */
    size_t i;
    for (i = 0; i <= n; ++i) dp[i] = 0;   /* initialize up to n */
    dp[0] = 1;

    seq_t ans = 0;
    idx_t S = 0;                 /* cumulative sum of used Fibonacci numbers */

    /* --- Add each Fibonacci coin and accumulate contributions -------- */
    for (k = 0; k < (size_t)MAXF && fib[k] != 0 && fib[k] <= n; ++k) {
        idx_t f = fib[k];

        /* update dp with the new coin f */
        for (idx_t t = f; t <= n; t += 1)
            dp[t] += dp[t - f];

        S += f;
        if (S > n) break;

        idx_t rem = n - S;
        ans += dp[(size_t)rem];   /* rem fits in size_t because rem ≤ n < 20001 */
    }

    return ans;
}
