/*
 * A022290  Replace 2^k in binary expansion of n with Fibonacci(k+2).
 * https://oeis.org/A022290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   a(n) = sum_{k>=0} [bit k of n is 1] * F(k+2), where F(k) is the
 *            Fibonacci sequence (F(0)=0, F(1)=1)
 * Range:     u8 n <= 1453, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function iterates over the bits of n and adds the corresponding
   Fibonacci numbers.  A static table of Fibonacci values up to F(65)
   is built once for all calls. */
seq_t a(idx_t n)
{
    static seq_t fib[66];
    static int ready = 0;
    if (!ready) {
        fib[0] = 0;
        fib[1] = 1;
        for (idx_t i = 2; i < 66; ++i) {
            fib[i] = fib[i-1] + fib[i-2];
        }
        ready = 1;
    }

    seq_t res = 0;
    idx_t bit = 0;
    while (n != 0) {
        if (n & 1) {
            res += fib[bit + 2];
        }
        n >>= 1;
        ++bit;
    }
    return res;
}
