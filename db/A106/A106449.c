/*
 * A106449  Square array (P(x) XOR P(y))/gcd(P(x),P(y)) where P(x) and P(y)
 *          are polynomials with coefficients in {0,1} given by the binary
 *          expansions of x and y, and all calculations are done in polynomial
 *          ring GF(2)[X], with the result converted back to a binary number,
 *          and then expressed in decimal. Array...
 * https://oeis.org/A106449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by antidiagonals from the square array A(x,y) =
 *            (P(x) ⊕ P(y)) / gcd(P(x),P(y)) where P(k) is the polynomial over
 *            GF(2) with coefficients given by the binary expansion of k and ⊕
 *            is carry‑less binary addition
 * Range:     u8 n <= 9436, u16 n <= 9436, u32 n <= 9436, u64 n <= 9436,
 *            u128 n <= 9436, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Utility: degree (highest set bit position, 0‑based)
   returns (unsigned int)-1 if x == 0 (used only for comparison).   */
static unsigned int poly_degree(seq_t x)
{
    unsigned int d = 0;
    while (x) {
        x >>= 1;
        ++d;
    }
    return d ? d - 1 : (unsigned int)-1;   /* -1 indicates zero polynomial */
}

/* ------------------------------------------------------------------ */
/*  Modulo of polynomials in GF(2):  a mod b   */
static seq_t poly_mod(seq_t a, seq_t b)
{
    if (b == (seq_t)0)
        return a;   /* undefined but never used with b=0 */

    unsigned int db = poly_degree(b);

    while (a != (seq_t)0) {
        unsigned int da = poly_degree(a);
        if (da < db)
            break;
        unsigned int shift = da - db;
        a ^= (b << shift);
    }
    return a;
}

/* ------------------------------------------------------------------ */
/*  GCD of polynomials in GF(2) using the Euclidean algorithm     */
static seq_t poly_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/*  Exact division: quotient of num by den, assuming den | num   */
static seq_t poly_div_exact(seq_t num, seq_t den)
{
    if (den == (seq_t)0)
        return (seq_t)0;   /* never happens for this sequence */

    seq_t quo = (seq_t)0;
    unsigned int db = poly_degree(den);

    while (num != (seq_t)0) {
        unsigned int dn = poly_degree(num);
        unsigned int shift = dn - db;
        quo ^= ((seq_t)1 << shift);
        num ^= (den << shift);
    }
    return quo;
}

/* ------------------------------------------------------------------ */
/*  Compute a(n) for the antidiagonal index n (offset 1).             */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)      /* not part of the sequence (offset 1) */
        return (seq_t)0;

    /* convert to zero‑based index for diagonal calculation          */
    idx_t n1 = n - (idx_t)1;

    /* find the diagonal t so that t*(t+1)/2 <= n1 < (t+1)*(t+2)/2   */
    idx_t t = 0;
    while (((t + (idx_t)1) * ((t + (idx_t)1) + (idx_t)1)) / (idx_t)2 <= n1)
        ++t;

    /* offset k inside this diagonal (0‑based)                       */
    idx_t k = n1 - t * (t + (idx_t)1) / (idx_t)2;

    /* coordinates are 1‑based: row = k+1, column = (t+2)-row      */
    idx_t row = k + (idx_t)1;
    idx_t col = (t + (idx_t)2) - row;

    /* convert integer coordinates to polynomials over GF(2).      */
    seq_t pv = (seq_t)row;   /* P(row) */
    seq_t pw = (seq_t)col;   /* P(col) */

    /* compute A(row,col) = (pv ⊕ pw) / gcd(pv,pw)                  */
    seq_t num = pv ^ pw;
    seq_t g   = poly_gcd(pv, pw);
    return poly_div_exact(num, g);
}
