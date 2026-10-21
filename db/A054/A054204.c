/*
 * A054204  Integers expressible as sums of distinct even-subscripted
 *          Fibonacci numbers.
 * https://oeis.org/A054204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{e≥0, bit e of n = 1} F_{2(e+1)} (Fibonacci numbers
 *            with F1=1, F2=1)
 * Range:     u8 n <= 63, u16 n <= 3295, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* static storage for Fibonacci numbers up to a fixed size  */
static seq_t fib_vals[200];
static unsigned int fib_len = 0;   /* number of Fibonacci values stored (index 1..fib_len) */

/* build Fibonacci numbers up to index 'upto' (inclusive) */
static void ensure_fib(unsigned int upto)
{
    if (fib_len == 0)
    {
        fib_vals[1] = (seq_t)1;
        fib_vals[2] = (seq_t)1;
        fib_len = 2;
    }
    while (fib_len <= upto && fib_len < 200)
    {
        fib_vals[fib_len + 1] = fib_vals[fib_len] + fib_vals[fib_len - 1];
        fib_len++;
    }
}

/* compute a(n) per the definition */
seq_t a(idx_t n)
{
    /* compute the highest bit position of n */
    unsigned int max_bit = 0;
    idx_t tmp = n;
    while (tmp > 1)
    {
        tmp >>= 1;
        max_bit++;
    }

    /* we need Fibonacci numbers up to index 2*(max_bit+1) */
    unsigned int need_fib = 2 * (max_bit + 1);
    ensure_fib(need_fib);

    seq_t ans = (seq_t)0;
    unsigned int e = 0;
    tmp = n;
    while (tmp)
    {
        if (tmp & 1)
            ans += fib_vals[2 * (e + 1)];
        tmp >>= 1;
        e++;
    }
    return ans;
}
