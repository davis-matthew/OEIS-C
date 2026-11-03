/*
 * A036999  Restricted permutations.
 * https://oeis.org/A036999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * (F(n+2) + 1) where F(k) is the k‑th Fibonacci number
 *            with F(0)=0, F(1)=1
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 982 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair_impl(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair_impl(n >> 1, &a, &b);      // (F(k), F(k+1))  for  k = n/2

    seq_t two_b = b + b;                // 2*F(k+1)
    seq_t c = a * (two_b - a);         // F(2k)
    seq_t d = a * a + b * b;           // F(2k+1)

    if ((n & 1) == 0) {                // n even
        *fn  = c;
        *fn1 = d;
    } else {                           // n odd
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    /* compute F(n+2) */
    idx_t m = n + 2;
    seq_t f, fp1;
    fib_pair_impl(m, &f, &fp1);

    seq_t result = f + (seq_t)1;
    result *= (seq_t)3;
    return result;
}
