/*
 * A055244  Number of certain stackings of n+1 squares on a double staircase.
 * https://oeis.org/A055244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+5)*F(n+1)+(2*n-3)*F(n))/5, with F(0)=0, F(1)=1
 *            (Fibonacci numbers)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 40, u64 n <= 85, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fibonacci via fast‑doubling.  F(0)=0, F(1)=1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);

    seq_t two_b = b * 2;
    seq_t c = a * (two_b - a);
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* a(0)=1, a(1)=1 are easy special cases – avoid negative
       multipliers that would appear for n = 0,1 in the formula. */
    if (n < 2)
        return 1;

    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);

    /* For 8‑byte or smaller seq_t types we use 128‑bit intermediates
       to guarantee exactness while still fitting the division. */
    if (sizeof(seq_t) <= 8) {
        /* 2*n-3 is positive for n>=2, but we keep the computation signed
           to avoid unsigned underflow for n = 0,1 (which are handled above). */
        __int128 coeff = ( (__int128)2 * (__int128)n ) - 3;
        __int128 term1 = (__int128)(n + 5) * (__int128)fn1;
        __int128 term2 = coeff * (__int128)fn;
        __int128 numer = term1 + term2;
        return (seq_t)(numer / 5);
    } else {
        /* seq_t is big‑integer: use its operators directly. */
        seq_t coeff = (seq_t)( (int64_t)2 * (int64_t)n - 3 );   /* positive for n>=2 */
        seq_t term1 = (seq_t)(n + 5) * fn1;
        seq_t term2 = coeff * fn;
        return (term1 + term2) / 5;
    }
}
