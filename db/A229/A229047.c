/*
 * A229047  Replace all '11' => '101' in the binary representation of n, treat
 *          the result as representation of a(n) in base of Fibonacci numbers
 *          (A014417).
 * https://oeis.org/A229047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = value of Fibonacci base number obtained by replacing each
 *            "11" in the binary representation of n by "101". The binary
 *            transformed string is interpreted as Zeckendorf (Fibonacci)
 *            representation with the rightmost digit having weight fib[1] =
 *            1, fib[2] = 2, fib[3] = 3, fib[4] = 5,
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Precompute Fibonacci weights fib[1] = 1, fib[2] = 2, fib[i] = fib[i-1]+fib[i-2] */
    static seq_t fib[64];
    static int fib_ready = 0;
    if (!fib_ready) {
        fib[1] = 1;
        fib[2] = 2;
        for (int i = 3; i < 64; ++i)
            fib[i] = fib[i - 1] + fib[i - 2];
        fib_ready = 1;
    }

    /* Step 1: build binary bits array (MSB to LSB) */
    idx_t tmp = n;
    /* special case n==0: treat as single 0 bit */
    size_t k = (tmp == 0) ? 1 : 0;
    idx_t bits[64];
    if (tmp == 0) {
        bits[0] = 0;
    } else {
        while (tmp > 0) {
            bits[k++] = tmp & 1;
            tmp >>= 1;
        }
    }

    /* Step 2: transform bits to output string as per rule */
    char out[128];
    size_t pos = 0;
    for (int i = (int)k - 1; i >= 0; --i) {
        int b  = bits[i];
        int nb = (i > 0) ? bits[i - 1] : 0;
        if (b && nb) {          /* "11" -> "10" */
            out[pos++] = '1';
            out[pos++] = '0';
        } else if (b) {         /* single "1" */
            out[pos++] = '1';
        } else {                /* "0" */
            out[pos++] = '0';
        }
    }
    size_t L = pos;

    /* Step 3: interpret as Fibonacci base (Zeckendorf) */
    seq_t result = 0;
    for (size_t j = 0; j < L; ++j) {
        if (out[j] == '1')
            result += fib[L - j];
    }
    return result;
}
