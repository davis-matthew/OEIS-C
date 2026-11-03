/*
 * A226205  a(n) = F(n)^2 - F(n-1)^2 or F(n+1) * F(n-2) where F(n) =
 *          A000045(n), the Fibonacci numbers.
 * https://oeis.org/A226205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = F(n)^2 - F(n-1)^2 where F(0)=0, F(1)=1 are the Fibonacci
 *            numbers
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 919 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair_internal(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;          /* F(0)   */
    seq_t b = 1;          /* F(1)   */
    if (n == 0) {         /* shortcut for n = 0 (not used by a() but safe) */
        *fn  = a;
        *fn1 = b;
        return;
    }

    /* Find the highest set bit of n to start the loop from that bit. */
    idx_t mask = (idx_t)1 << ((sizeof(idx_t) * 8) - 1);
    while ((mask & n) == 0)
        mask >>= 1;

    for (; mask; mask >>= 1) {
        /* Doubling formulas:
           c = F(2k)   = F(k) · (2·F(k+1) – F(k))
           d = F(2k+1) = F(k)^2 + F(k+1)^2
        */
        seq_t two_b = b << 1;              /* 2·F(k+1) */
        seq_t c = a * (two_b - a);
        seq_t d = a * a + b * b;

        if (n & mask) {                    /* use F(2k+1), F(2k+2) */
            a = d;
            b = c + d;
        } else {                           /* use F(2k),   F(2k+1) */
            a = c;
            b = d;
        }
    }

    *fn  = a;   /* F(n)   */
    *fn1 = b;   /* F(n+1) */
}
/* Implements fast doubling Fibonacci to compute F(n) and F(n+1) in
   logarithmic time. */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1, but the implementation
       accepts n == 0 as returning 0 for safety. */
    if (n == 0)
        return 0;

    seq_t fn, fn1;
    fib_pair_internal((idx_t)(n), &fn, &fn1);   /* fn  = F(n),   fn1 = F(n+1) */

    seq_t fnm1 = fn1 - fn;                       /* F(n-1) = F(n+1) - F(n) */

    return fn * fn - fnm1 * fnm1;                /* F(n)^2 - F(n-1)^2 */
}
