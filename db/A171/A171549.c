/*
 * A171549  a(n) is the n-th "strange number", where "strange numbers" are
 *          defined as follows: using every other Fibonacci number (starting
 *          with offset 1), the shortest way to add up these Fibonacci numbers
 *          so that the sum equals n, where the digits are indices in the
 *          "every other Fibonacci number" sequence.
 * https://oeis.org/A171549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: greedy representation of n using odd‑index Fibonacci numbers (F1, F3, F5, …) with minimal number of terms.  
   The result is the concatenation of the indices (1‑based) of the chosen Fibonacci numbers, written in descending order. */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* generate odd‑index Fibonacci numbers up to n */
    seq_t fib[128];
    int fibCount = 0;
    seq_t a_fib = 1, b_fib = 1;   /* F1 = 1, F2 = 1 */
    idx_t i = 1;                  /* current Fibonacci index */
    while (a_fib <= n)
    {
        if (i & 1)               /* odd index */
            fib[fibCount++] = a_fib;
        seq_t next = a_fib + b_fib;
        a_fib = b_fib;
        b_fib = next;
        i++;
    }

    seq_t result = 0;
    seq_t remaining = n;

    while (remaining > 0)
    {
        /* find the largest odd‑index Fibonacci number ≤ remaining */
        int k = fibCount - 1;
        while (fib[k] > remaining)
            k--;

        remaining -= fib[k];
        int idx = k + 1;              /* 1‑based index in the odd‑index sequence */
        int digits = (idx < 10)      ? 1 :
                     (idx < 100)     ? 2 : 3;   /* works for our range */

        seq_t mult = pow10((idx_t)digits);
        result = result * mult + (seq_t)idx;
    }

    return result;
}
