/*
 * A144157  Eigentriangle, row sums = A011782: (1, 1, 2, 4, 8, 16, ...).
 * https://oeis.org/A144157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let i = ⌊(√(8n+1)‑1)/2⌋, j = n – i(i+1)/2. Define f(0)=1,
 *            f(1)=0, and for k≥2: f(k)=f(k‑1)+f(k‑2). Then if j==0: a(n)=f(i)
 *            if j==1: a(n)=f(i‑1) otherwise: a(n)=f(i‑j) * 2^(j‑2) This
 *            reproduces OEIS A144157
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Fibonacci fast doubling: returns F_n and F_{n+1} in *Fn and *Fn1 */
static void fib_fast_doubling(idx_t n, seq_t *Fn, seq_t *Fn1)
{
    if (n == 0) {
        *Fn = (seq_t)0;
        *Fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_fast_doubling(n >> 1, &a, &b);          /* a=F_k, b=F_{k+1} */

    seq_t c = a * (b + b - a);                  /* c = F_{2k}   = F_k*(2*F_{k+1}−F_k) */
    seq_t d = a * a + b * b;                    /* d = F_{2k+1} = F_k^2 + F_{k+1}^2 */

    if ((n & 1) == 0) {
        *Fn = c;
        *Fn1 = d;
    } else {
        *Fn = d;
        *Fn1 = c + d;
    }
}

/* f(k): returns sequence value at index k (k≥0) */
static seq_t get_f(idx_t k)
{
    if (k == 0) return (seq_t)1;
    seq_t Fn, _;
    fib_fast_doubling(k - 1, &Fn, &_);
    return Fn;          /* Fn = F_{k-1} */
}

/* pow2(e): returns 2^e as seq_t (e≥0) using exponentiation by squaring */
static seq_t pow2_exp(idx_t e)
{
    seq_t result = (seq_t)1;
    seq_t base = (seq_t)2;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* compute i = floor((sqrt(8n+1)-1)/2) */
    unsigned __int128 val = (unsigned __int128)n * 8 + 1;
    double d = sqrt((double)val);
    idx_t s = (idx_t)d;
    while ((unsigned __int128)(s + 1) * (s + 1) <= val) ++s;
    while ((unsigned __int128)s * s > val) --s;
    idx_t i = (s - 1) / 2;

    /* row start index Ti = i*(i+1)/2 */
    unsigned __int128 Ti64 = (unsigned __int128)i * (i + 1) / 2;
    idx_t Ti = (idx_t)Ti64;
    idx_t j = n - Ti;      /* 0 <= j <= i */

    if (j == 0) {
        return get_f(i);
    } else if (j == 1) {
        return get_f(i - 1);
    } else {
        seq_t fk = get_f(i - j);
        idx_t exp = j - 2;
        seq_t pow2 = pow2_exp(exp);
        return fk * pow2;
    }
}
