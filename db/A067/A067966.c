/*
 * A067966  Number of binary arrangements without adjacent 1's on n X n array
 *          connected n-s.
 * https://oeis.org/A067966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = F(n+2)^n where F(k) is the k‑th Fibonacci number (F(0)=0,
 *            F(1)=1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Fibonacci pair (F(n), F(n+1)) by fast doubling.  The algorithm works for all
   unsigned integer n and uses only integer arithmetic on seq_t values. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1)  (k = n/2) */

    seq_t two_b = b + b;                /* 2*F(k+1) */
    seq_t c = a * (two_b - a);         /* F(2k)     = F(k) * (2*F(k+1) - F(k)) */
    seq_t d = a * a + b * b;           /* F(2k+1)   = F(k)^2 + F(k+1)^2 */

    if ((n & 1) == 0) {                /* n even  ->  F(n)  = F(2k)     */
        *fn  = c;
        *fn1 = d;
    } else {                           /* n odd   ->  F(n)  = F(2k+1)   */
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Power function: base^exp, exp is idx_t.  Uses exponentiation by squaring. */
static seq_t power_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result = result * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }
    return result;
}

/* Public function required by the tester. */
seq_t a(idx_t n)
{
    seq_t fib_val;
    seq_t dummy;
    /* Compute F(n+2).  The second returned value is unused. */
    fib_pair(n + 2, &fib_val, &dummy);
    return power_seq_t(fib_val, n);
}
