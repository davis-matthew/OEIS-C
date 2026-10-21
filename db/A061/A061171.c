/*
 * A061171  One half of second column of Lucas bisection triangle (odd part).
 * https://oeis.org/A061171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+4) + n * Lucas(2*n+3) (Lucas(2*n+3) =
 *            F(2*n+2)+F(2*n+4) = F(k-2)+F(k) for k = 2*n+4)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 40, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *f, seq_t *f1) {
    /* Fast doubling: returns (F(n), F(n+1)) */
    if (n == 0) {
        *f = 0;
        *f1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);
    /* c = F(2k) = F(k) * (2*F(k+1) - F(k)) */
    seq_t two_b = 2 * b;
    seq_t two_b_minus_a = two_b - a;
    seq_t c = a * two_b_minus_a;
    /* d = F(2k+1) = F(k+1)^2 + F(k)^2 */
    seq_t d = a * a + b * b;
    if ((n & 1) == 0) {
        *f = c;
        *f1 = d;
    } else {
        *f = d;
        *f1 = c + d;
    }
}

seq_t a(idx_t n) {
    /* Compute k = 2*n + 4 (fits into idx_t for all practical n) */
    idx_t k = (n << 1) + 4;

    seq_t Fk, Fk1;
    fib_pair(k, &Fk, &Fk1);   /* F(k), F(k+1) */

    /* F(k-1) = F(k+1) - F(k)  (non‑negative because F(k+1) ≥ F(k)) */
    seq_t Fk_minus1 = Fk1 - Fk;
    /* F(k-2) = F(k) - F(k-1) */
    seq_t Fk_minus2 = Fk - Fk_minus1;

    /* Lucas(2*n+3) = L(k-1) = F(k-2) + F(k) */
    seq_t lucas = Fk_minus2 + Fk;

    /* a(n) = F(k) + n * L(k-1) */
    return Fk + ((seq_t)n * lucas);
}
