/*
 * A136185  Number of metacyclic groups of order p^n, prime p >= 3.
 * https://oeis.org/A136185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 12*n^2 + c*n + d) / 72
 * Range:     u8 n <= 3, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where c = 12 if n is even, else 3;                */
/* and d = 72 if n ≡ 0 (mod 3), 56 if n ≡ 1 (mod 3), */
/*     or 64 if n ≡ 2 (mod 3). This implements the */
/*     closed form given in the OEIS entry for A136185. */

seq_t a(idx_t n)
{
    /* Convert to seq_t to avoid accidental overflow in intermediate */
    seq_t x = (seq_t)n;

    /* Compute c based on parity of n */
    seq_t c = (n & 1) ? 3 : 12;  /* n%2 */
    /* Compute d based on n mod 3 */
    int r = n % 3;
    seq_t d;
    if (r == 0)          d = 72;
    else if (r == 1)     d = 56;
    else                 d = 64;   /* r == 2 */

    /* Compute numerator = n^3 + 12*n^2 + c*n + d */
    seq_t n2 = x * x;              /* n^2 */
    seq_t n3 = n2 * x;             /* n^3 */
    seq_t term = n3 + 12 * n2 + c * x + d;

    return term / 72;
}
