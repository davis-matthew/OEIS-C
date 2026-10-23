/*
 * A014808  Inverse of 799th cyclotomic polynomial.
 * https://oeis.org/A014808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Inverse of the 799th cyclotomic polynomial.
    The 1/F_799(x) series has period 799.
    Compute Φ_799(x) = (x-1)(x^799-1)/((x^17-1)(x^47-1))
    by exact integer long division.  Let Φ_799(x) = Σ_{k=0}^{736} c_k x^k
    (c_0 = 1).  Then the inverses a(n) satisfy the linear recurrence

        a(0) = 1,
        a(n) = - Σ_{k=1}^{736} c_k · a(n-k)       (n ≥ 1)

    The array a[0…798] is computed once and the sequence is repeated
    with period 799.  This satisfies all OEIS requirements.  */

#include <stdbool.h>

#define PERIOD 799
#define DEG 736   /* degree of Φ_799(x) */

static seq_t coeffs[DEG + 1];   /* c_k of Φ_799(x) */
static seq_t seqA[PERIOD];      /* a(n) for 0 ≤ n < 799 */
static bool initialized = false;

/* exact integer long division: numerator / denominator = quotient  */
static void init(void)
{
    if (initialized)
        return;

    /* numerator = (x^800 - x^799 - x + 1) */
    seq_t num[801] = {0};
    num[800] = 1;
    num[799] = -1;
    num[1]   = -1;
    num[0]   = 1;

    /* denominator = (x^64 - x^47 - x^17 + 1) */
    seq_t den[65] = {0};
    den[64] = 1;
    den[47] = -1;
    den[17] = -1;
    den[0]  = 1;

    /* long division: num / den -> quotient stored in coeffs */
    for (int i = 800; i >= 64; --i) {
        seq_t q = num[i];
        if (q != 0) {
            int qdeg = i - 64;          /* quotient degree */
            coeffs[qdeg] = q;           /* unique quotient coefficient */
            for (int j = 0; j <= 64; ++j) {
                num[qdeg + j] -= q * den[j];
            }
        }
    }
    /* remainder must be zero */

    /* build the first PERIOD terms of the series */
    seqA[0] = 1;                   /* a(0) = 1 */
    for (int n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        int kmax = (n <= DEG) ? n : DEG;   /* only existing coefficients */
        for (int k = 1; k <= kmax; ++k) {
            sum -= coeffs[k] * seqA[n - k];
        }
        seqA[n] = sum;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init();
    return seqA[n % PERIOD];
}
