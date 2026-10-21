/*
 * A081019  a(n) = Lucas(4n+3) - 1, or Lucas(2n+1)*Lucas(2n+2).
 * https://oeis.org/A081019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(4n + 3) - 1 (equivalently a(n) = Lucas(2n+1) *
 *            Lucas(2n+2) )
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute pair (F(n), F(n+1)) using fast doubling */
static void fib_pair(idx_t n, seq_t *f, seq_t *fn1)
{
    if (n == 0) {
        *f = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);            /* (F(k), F(k+1)) where k = floor(n/2) */
    seq_t two_b_minus_a = ((b << 1) - a);
    seq_t c = a * two_b_minus_a;          /* F(2k)   = F(k) * (2*F(k+1) - F(k)) */
    seq_t d = a * a + b * b;             /* F(2k+1) = F(k)^2 + F(k+1)^2 */
    if (n & 1) {
        *f = d;
        *fn1 = c + d;
    } else {
        *f = c;
        *fn1 = d;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute Lucas(4n+3) = 2*F(4n+4) - F(4n+3) */
    idx_t m = (n << 2) + 3;               /* 4*n + 3 */
    seq_t f, fn1;
    fib_pair(m, &f, &fn1);                /* f = F(m), fn1 = F(m+1) */
    seq_t lucas = (fn1 << 1) - f;         /* L(m) = 2*F(m+1) - F(m) */
    return lucas - (seq_t)1;             /* a(n) = L(m) - 1 */
}
