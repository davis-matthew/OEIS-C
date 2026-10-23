/*
 * A105870  Fibonacci sequence (mod 7).
 * https://oeis.org/A105870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, hear
 * Formula:   a(0) = 0; a(1) = 1; a(n) = (a(n-1) + a(n-2)) mod 7 (n >= 2)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);            /* a = F(k), b = F(k+1) where k = n/2 */

    /* Modulo value 7 */
    const seq_t MOD = 7;

    /* Compute c = F(2k) = a * (2*b - a) mod MOD */
    seq_t a_mod = a % MOD;
    seq_t b_mod = b % MOD;
    seq_t two_b = ((seq_t)2 * b_mod) % MOD;
    seq_t tmp   = (two_b + MOD - a_mod) % MOD;  /* ensure non‑negative */
    seq_t c     = (a_mod * tmp) % MOD;

    /* Compute d = F(2k+1) = a*a + b*b mod MOD */
    seq_t d = (a_mod * a_mod + b_mod * b_mod) % MOD;

    if ((n & 1) == 0) {          /* n is even: F(n)=c, F(n+1)=d */
        *fn  = c;
        *fn1 = d;
    } else {                     /* n is odd: F(n)=d, F(n+1)=c+d */
        *fn  = d;
        *fn1 = (c + d) % MOD;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn_next;
    fib_pair(n, &fn, &fn_next);
    return fn;
}
