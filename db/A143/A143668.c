/*
 * A143668  Result of the morphing 01->01021212, 02->0102121201, 12->01021201,
 *          iterated from '01'. Sequence of the Fibonacci word fractal.
 * https://oeis.org/A143668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let (b(n)) be the n-th term of the Fibonacci word. a(n) = 2 if
 *            b(n)=0 and n is even; otherwise a(n) = b(n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Preserve original index for parity test */
    idx_t orig = n;

    /* Fast find of the smallest k such that length of W_k >= n.
       Length of W_k is fib[k+2], where fib[0] = 0, fib[1] = 1.
       We use 64‑bit arithmetic and stop when an addition would overflow. */
    uint64_t f_prev = 0, f_cur = 1; /* fib[i], fib[i+1] */
    int k = 0;                      /* current i, also the desired k */
    while (1) {
        /* If next addition would overflow, the following Fibonacci number
           would be >UINT64_MAX >= n, so we can break here. */
        if (f_cur > UINT64_MAX - f_prev) {
            break; /* next Fibonacci number would overflow */
        }
        uint64_t f_len = f_prev + f_cur; /* fib[i+2] */
        if (f_len >= n) {
            break;  /* found minimal k = i such that length >= n */
        }
        f_prev = f_cur;
        f_cur = f_len;
        ++k;
    }

    /* Build Fibonacci table up to fib[93] once per program run.
       fib[93] = 12200160415121876738 fits in 64 bits. */
    static uint64_t fib[94];
    static int fib_ready = 0;
    if (!fib_ready) {
        fib_ready = 1;
        fib[0] = 0;
        fib[1] = 1;
        for (int i = 2; i < 94; ++i) {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
    }

    /* Recursively determine the bit b(n) of the Fibonacci word */
    idx_t pos = n;
    int curk = k;

    while (curk > 1) {
        /* Length of the left part (W_{curk-1}) is fib[curk+1] */
        uint64_t left_len = fib[curk + 1];
        if (pos <= left_len) {
            curk -= 1;          /* go to the left child */
        } else {
            pos -= left_len;    /* skip the left part */
            curk -= 2;          /* go to the right child */
        }
    }

    /* curk is now 0 or 1 – base cases of the Fibonacci word */
    seq_t bit = (curk == 0) ? (seq_t)0 :
                (pos == 1 ? (seq_t)0 : (seq_t)1);

    /* Apply the replacement rule: 0 → 2 on even positions */
    if (bit == 0 && ((orig & 1u) == 0)) {
        return (seq_t)2;
    }
    return bit;
}
