/*
 * A165276  Number of even-indexed Fibonacci numbers in the Zeckendorf
 *          representation of n.
 * https://oeis.org/A165276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): number of even‑indexed Fibonacci numbers in the Zeckendorf representation of n. */
seq_t a(idx_t n)
{
    idx_t fib[200];
    fib[0] = 1;
    fib[1] = 2;
    idx_t count = 2;

    /* generate Fibonacci numbers <= n, stop before overflow or exceeding n */
    while (count < 200) {
        idx_t a = fib[count - 1];
        idx_t b = fib[count - 2];
        if (a > n - b) {                 /* a + b would exceed n or overflow */
            break;
        }
        idx_t nxt = a + b;
        fib[count++] = nxt;
    }

    idx_t rem = n;
    seq_t res = 0;
    for (idx_t i = count; i-- > 0; ) {
        if (fib[i] <= rem) {
            rem -= fib[i];
            if ((i & 1) == 0) {         /* i even ⇒ Fibonacci index (i+2) even */
                res++;
            }
        }
    }
    return res;
}
