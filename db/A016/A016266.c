/*
 * A016266  Inverse of 2257th cyclotomic polynomial.
 * https://oeis.org/A016266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of x^n in the power series expansion of 1 /
 *            Φ2257(x), where Φ2257 is the 2257‑th cyclotomic polynomial. The
 *            sequence is periodic with period 2257
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* calloc, free, abort */

#define PERIOD 2257
#define DEG_A 2258          /* degree of (x^2257-1)*(x-1) = x^2258 - x^2257 - x + 1 */
#define DEG_D 98            /* degree of (x^37-1)*(x^61-1) = x^98 - x^37 - x^61 + 1 */
#define DEG_Q 2160          /* degree of Φ2257(x)  = DEG_A - DEG_D */

/* ----------------------------------------------------------------- */
/*  Compute the coefficients of Φ2257(x).  Result stored in global
    array F[0..DEG_Q] (degree 2160).                                    */
static seq_t *F = NULL;          /* coefficients of Φ2257   */
static int f_computed = 0;       /* 0 => not yet computed */

/* Allocate memory for an array of seq_t of the given size */
static void *alloc_seq_array(size_t n)
{
    return calloc(n, sizeof(seq_t));
}

static void compute_F(void)
{
    if (f_computed) return;

    F = (seq_t *)alloc_seq_array(DEG_Q + 1);
    if (!F) abort();

    /* Numerator A(x) = (x^2257-1)(x-1) = x^2258 - x^2257 - x + 1   */
    seq_t *rem = (seq_t *)alloc_seq_array(DEG_A + 1);
    if (!rem) abort();
    rem[0]      = 1;                 /* term of x^0      */
    rem[1]      = -1;                /* term of x^1      */
    rem[2257]   = -1;                /* term of x^2257   */
    rem[2258]   = 1;                 /* term of x^2258   */

    /* Denominator D(x) = (x^37-1)(x^61-1) = x^98 - x^37 - x^61 + 1  */
    seq_t D[DEG_D + 1] = {0};
    D[0]   = 1;
    D[37]  = -1;
    D[61]  = -1;
    D[98]  = 1;

    /* Long division: A(x) / D(x)  →  quotient = Φ2257(x) */
    seq_t *quot = (seq_t *)alloc_seq_array(DEG_Q + 1);
    if (!quot) abort();

    for (int i = DEG_A; i >= DEG_D; --i) {
        seq_t div = rem[(size_t)i];
        size_t pos = (size_t)(i - DEG_D);
        quot[pos] = div;
        for (size_t j = 0; j <= DEG_D; ++j) {
            rem[pos + j] -= div * D[j];
        }
    }

    /* Copy quotient into global array F */
    for (size_t i = 0; i <= DEG_Q; ++i) {
        F[i] = quot[i];
    }

    free(rem);
    free(quot);
    f_computed = 1;
}

/* ----------------------------------------------------------------- */
/*  Compute the first PERIOD terms of a(n).  They are stored in
    global array A[0..PERIOD-1].                                       */
static seq_t *A = NULL;          /* periodic coefficients   */
static int a_computed = 0;       /* 0 => not yet computed */

static void compute_sequence(void)
{
    if (a_computed) return;

    compute_F();   /* ensure F is ready */

    A = (seq_t *)alloc_seq_array(PERIOD);
    if (!A) abort();

    A[0] = 1;
    for (size_t n = 1; n < (size_t)PERIOD; ++n) {
        seq_t sum = 0;
        size_t maxj = (n <= (size_t)DEG_Q) ? n : (size_t)DEG_Q;
        for (size_t j = 1; j <= maxj; ++j) {
            sum += F[j] * A[n - j];
        }
        A[n] = -sum;   /* recurrence: a(n) + Σ_{j≥1} F[j] a(n-j) = 0 */
    }

    a_computed = 1;
}

/* ----------------------------------------------------------------- */
/*  Public sequence function                                            */
seq_t a(idx_t n)
{
    compute_sequence();
    size_t idx = (size_t)(n % PERIOD);
    return A[idx];
}
