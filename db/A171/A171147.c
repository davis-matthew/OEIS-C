/*
 * A171147  The sequence of coefficients of a polynomial recursion:
 *          p(x,n)=If[Mod[n, 2] == 0, (x + 1)*p(x, n - 1), (x^2 + (2*n)*x +
 *          1)^Floor[n/2]].
 * https://oeis.org/A171147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^(k) in the polynomial p(x,n) defined by
 *            p(x,1) = 1, p(x,n) = (x + 1) * p(x, n-1) (n even) p(x,n) = (x^2
 *            + (2*n)*x + 1)^(⌊n/2⌋) (n odd) The sequence lists all
 *            coefficients of the polynomials row by row. Row r contains r
 *            terms; the first term of row r is a(T(r‑1)+1) (T(m)=m(...
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ---------- constants ---------- */
#define MAX_ROWS   200          /* enough for n up to 10000 (r≈141)     */
#define MAX_DEG   400           /* maximum possible degree of a row     */

/* ---------- storage for coefficients ---------- */
static seq_t  row[MAX_ROWS+1][MAX_DEG];     /* row[1]..row[MAX_ROWS]          */
static idx_t  rowlen[MAX_ROWS+1] = {0};     /* actual length of each row       */
static idx_t  maxRowComputed = 0;           /* highest row fully computed      */

/* ---------- helper: polynomial multiplication ----------
 * Multiply polynomials A[0..lenA-1] and B[0..lenB-1].
 * Result is stored in C[0..lenC-1], with lenC returned.
 * All arrays are at least MAX_DEG long.                */
static void poly_mul(const seq_t *A, idx_t lenA,
                     const seq_t *B, idx_t lenB,
                     seq_t *C, idx_t *lenC)
{
    idx_t i, j;
    idx_t resultLen = lenA + lenB - 1;
    for (i = 0; i < resultLen; ++i) C[i] = 0;
    for (i = 0; i < lenA; ++i) {
        for (j = 0; j < lenB; ++j) {
            C[i + j] += A[i] * B[j];
        }
    }
    *lenC = resultLen;
}

/* ---------- helper: polynomial exponentiation ----------
 * Raise polynomial base[0..lenBase-1] to exponent exp (integer >=0).
 * Result is stored in res[0..] with length resLen. */
static void poly_pow(const seq_t *base, idx_t lenBase,
                     idx_t exp,
                     seq_t *res, idx_t *resLen)
{
    seq_t temp1[MAX_DEG*2];
    seq_t temp2[MAX_DEG*2];
    seq_t resPoly[MAX_DEG*2];
    seq_t basePoly[MAX_DEG*2];
    idx_t lenResPoly, lenBasePoly;

    /* initialise result = 1 */
    for (int i=0;i<2*MAX_DEG;i++) resPoly[i]=0;
    resPoly[0] = 1;  lenResPoly = 1;

    /* initialise base */
    for (int i=0;i<2*MAX_DEG;i++) basePoly[i]=0;
    for (idx_t i=0;i<lenBase;i++) basePoly[i] = base[i];
    lenBasePoly = lenBase;

    while (exp > 0) {
        if (exp & 1) {
            poly_mul(resPoly, lenResPoly, basePoly, lenBasePoly,
                     temp1, &lenResPoly);
            for (idx_t i = 0; i < lenResPoly; ++i) resPoly[i] = temp1[i];
        }
        /* square the base */
        poly_mul(basePoly, lenBasePoly, basePoly, lenBasePoly,
                 temp2, &lenBasePoly);
        for (idx_t i = 0; i < lenBasePoly; ++i) basePoly[i] = temp2[i];
        exp >>= 1;
    }
    /* copy result */
    for (idx_t i=0;i<lenResPoly;i++) res[i] = resPoly[i];
    *resLen = lenResPoly;
}

/* ---------- compute polynomial for a row ----------
 * Fills row[r][0..len-1] and sets rowlen[r].
 * r >= 1.  Previous rows are assumed to be already computed.   */
static void compute_row(idx_t r)
{
    if (r == 1) {
        row[1][0] = 1;
        rowlen[1] = 1;
        return;
    }

    if (r % 2 == 0) {               /* even: multiply by (x + 1) */
        idx_t prev = r - 1;
        idx_t lenPrev = rowlen[prev];
        rowlen[r] = lenPrev + 1;
        /* copy lower part */
        for (idx_t i = 0; i < lenPrev; ++i) row[r][i] = row[prev][i];
        /* add shifted part */
        for (idx_t i = 1; i < lenPrev + 1; ++i)
            row[r][i] += row[prev][i - 1];
    } else {                         /* odd: (x^2 + (2r)x + 1)^(r/2) */
        idx_t k = r / 2;   /* floor(r/2) */
        seq_t base[3];
        base[0] = 1;
        base[1] = (seq_t)(2 * r);
        base[2] = 1;
        seq_t result[MAX_DEG*2];
        idx_t lenRes;
        poly_pow(base, 3, k, result, &lenRes);
        rowlen[r] = lenRes;
        for (idx_t i = 0; i < lenRes; ++i) row[r][i] = result[i];
    }
}

/* ---------- main function a(n) ----------
 * Returns the nth term of the sequence (offset 1).           */
seq_t a(idx_t n)
{
    /* find row r such that n lies in it */
    idx_t r = 1;
    while ((seq_t)r * (r + 1) / 2 < n) ++r;

    /* compute rows up to r */
    while (maxRowComputed < r) {
        ++maxRowComputed;
        compute_row(maxRowComputed);
    }

    /* position within row r */
    idx_t prevSum = r * (r - 1) / 2;   /* T(r-1) */
    idx_t pos = n - prevSum - 1;       /* 0‑based */

    return row[r][pos];
}
