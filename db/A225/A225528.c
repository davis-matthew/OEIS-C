/*
 * A225528  a(n) = sigma(n)*Lucas(n) where Lucas(n) = A000204(n) and sigma(n)
 *          = A000203(n) is the sum of divisors of n.
 * https://oeis.org/A225528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) * Lucas(n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 37, u64 n <= 82, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas(n) is Lucas sequence: L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2). */
/* sigma(n) is the sum of divisors of n. */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Fast doubling Fibonacci: returns F(n) in *fn and F(n+1) in *fn1.  */
/* Works for any unsigned integer type seq_t (including arbitrary-     */
/* precision).                                                        */
/* ------------------------------------------------------------------ */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;           /* F(0) */
    seq_t b = 1;           /* F(1) */
    if (n == 0) {
        *fn  = a;
        *fn1 = b;
        return;
    }
    /* Find most significant bit of n */
    idx_t t = n;
    int msb = 0;
    while (t >> 1) {
        t >>= 1;
        msb++;
    }
    for (int i = msb; i >= 0; --i) {
        /* c = F(2k) = F(k) * (2*F(k+1) - F(k)) */
        seq_t c = a * (((seq_t)2 * b) - a);
        /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
        seq_t d = a * a + b * b;
        if ((n >> i) & 1) {
            a = d;
            b = c + d;
        } else {
            a = c;
            b = d;
        }
    }
    *fn  = a;
    *fn1 = b;
}

/* ------------------------------------------------------------------ */
/* Compute Lucas number L(n).                                          */
/* ------------------------------------------------------------------ */
static seq_t lucas(idx_t n)
{
    if (n == 0)       return (seq_t)2;   /* L(0) = 2 */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);              /* fn = F(n), fn1 = F(n+1) */
    /* F(n-1) = F(n+1) - F(n) */
    seq_t fn_minus1 = fn1 - fn;
    return fn_minus1 + fn1;              /* L(n) = F(n-1) + F(n+1) */
}

/* ------------------------------------------------------------------ */
/* Sum of divisors σ(n) for n >= 1.  Trial division up to √n.        */
/* ------------------------------------------------------------------ */
static seq_t sigma_of(idx_t n)
{
    seq_t result = 1;
    idx_t temp   = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            seq_t term = 1;
            seq_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
                term += power;
            }
            result *= term;
        }
    }
    if (temp > 1) {                       /* Remaining prime factor */
        result *= (seq_t)(1 + temp);
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n is 1-indexed as per OEIS offset. */
    seq_t sig   = sigma_of(n);
    seq_t luc   = lucas(n);
    return sig * luc;
}
