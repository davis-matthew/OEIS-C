/*
 * A015144  Triangle of (Gaussian) q-binomial coefficients for q=-19.
 * https://oeis.org/A015144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = the q‑binomial coefficient [r choose k]_{q=-19}, where r
 *            and k are obtained by flattening the triangular array of
 *            Gaussian binomials. r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2 and
 *            k = n – r(r+1)/2
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 22, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard facilities.  No <math.h> is used.  */
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Auxiliary: gcd for arbitrary integer type seq_t.  Works for
   negative operands.  */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    seq_t aa = a < 0 ? -a : a;
    seq_t bb = b < 0 ? -b : b;
    while (bb != 0) {
        seq_t t = aa % bb;
        aa = bb;
        bb = t;
    }
    return aa;
}

/* ------------------------------------------------------------------ */
/* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.
   Returns r.  Complexity O(r).  */
static inline idx_t find_row(idx_t n)
{
    idx_t r = 0;
    /* use 128‑bit intermediate to avoid overflow */
    while (((__int128)(r + 1) * (r + 2)) / 2 <= n)
        r++;
    return r;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* 1. Determine row r and column k.  */
    idx_t r = find_row(n);
    idx_t start = r * (r + 1) / 2;   /* first index in row r */
    idx_t k = n - start;
    if (k > r - k)                    /* use symmetry to minimise loops */
        k = r - k;

    /* 2. Pre‑compute powers of q = -19 up to exponent r.  */
    seq_t *powq = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!powq) return 0;                     /* out‑of‑memory guard */
    powq[0] = 1;
    const seq_t q = -19;
    for (idx_t i = 1; i <= r; ++i)
        powq[i] = powq[i - 1] * q;

    /* 3. Compute Gaussian coefficient multiplicatively,
       canceling common factors to keep intermediate values small. */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Numerator: 1 - q^(r-k+i)   */
        seq_t numer = powq[r - k + i] - 1;
        /* Denominator: 1 - q^i */
        seq_t denom = powq[i] - 1;

        /* Cancel gcd between numer and denom */
        seq_t g1 = seq_gcd(numer, denom);
        numer /= g1;
        denom /= g1;

        /* Cancel any remaining common factor with current result */
        seq_t g2 = seq_gcd(result, denom);
        result /= g2;
        denom /= g2;

        /* At this point denom is ±1.  Multiply result by numer. */
        seq_t tmp = result * numer;
        if (denom == -1) tmp = -tmp;
        result = tmp;
    }

    free(powq);
    return result;
}
