/*
 * A060112  Sums of nonconsecutive factorial numbers.
 * https://oeis.org/A060112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 55, u32 n <= 377, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A060112 – Sums of nonconsecutive factorial numbers
 *
 *  a(n) = Σ_{i : “bit i” is 1 in the Zeckendorf expansion of (n-1)}
 *         (i+1)!            (with Fib[0] = 1, Fib[1] = 2, …)
 *
 *  The algorithm:
 *  • Precompute Fibonacci numbers (1, 2, 3, 5, …) up to overflow.
 *  • For the given n, reduce to m = n-1.
 *  • Scan the Fibonacci array from the largest downwards:
 *        if fib[i] ≤ m   →  take it (subtract, add (i+1)!),
 *        then skip the next Fibonacci to preserve the Zeckendorf
 *        representation (no two consecutive Fibonacci numbers).
 *  • Factorials are cached incrementally to avoid recomputation.
 *
 *  All operations use only integer arithmetic and fit the constraints
 *  for C11 and C++17. */

#include <stddef.h>   /* size_t */

static seq_t fact_cache[256] = { 1, 1 }; /* factorial[0] = 1, factorial[1] = 1 */
static idx_t fact_max = 1;                /* highest factorial index computed */

static void ensure_fact(idx_t k)
{
    if (k <= fact_max)
        return;
    seq_t f = fact_cache[fact_max];
    for (idx_t i = fact_max + 1; i <= k; ++i) {
        f *= i;
        fact_cache[i] = f;
    }
    fact_max = k;
}

/* Fibonacci numbers 1,2,3,5,8,… used for Zeckendorf representation. */
static idx_t fib[94];
static size_t fib_len = 0;

static void init_fib(void)
{
    if (fib_len != 0)
        return;
    fib[0] = 1;
    fib[1] = 2;
    fib_len = 2;
    for (size_t i = 2; i < 94; ++i) {
        idx_t next = fib[i - 1] + fib[i - 2];
        if (next < fib[i - 1]) /* overflow */
            break;
        fib[i] = next;
        ++fib_len;
    }
}

seq_t a(idx_t n)
{
    /* convert to Zeckendorf domain */
    if (n == 0)
        return 0;           /* undefined, but keep safe */
    idx_t m = n - 1;        /* Zeckendorf uses n-1 */

    init_fib();

    seq_t sum = 0;
    int i = (int)fib_len - 1;      /* start from the largest Fibonacci */

    while (m > 0 && i >= 0) {
        if (fib[i] <= m) {
            m -= fib[i];
            ensure_fact((idx_t)i + 1);     /* (i+1)! */
            sum += fact_cache[(idx_t)i + 1];
            i -= 2;                          /* skip consecutive Fibonacci */
        } else {
            --i;
        }
    }

    return sum;
}
