/*
 * A014412  Inverse of 403rd cyclotomic polynomial.
 * https://oeis.org/A014412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power series expansion
 *            of
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1 / Φ403(x) , where Φ403(x) is the 403rd cyclotomic polynomial            */

#include <stdbool.h>
#include <string.h>

#define PERIOD 403

static seq_t periodArray[PERIOD];
static bool period_initialized = false;

static void init_seq(void)
{
    const int Ndeg = 404;          /* degree of N(x) = (x-1)(x^403-1) */
    const int Ddeg = 44;           /* degree of D(x) = (x^13-1)(x^31-1) */
    const int Qdeg = Ndeg - Ddeg;  /* degree of Φ403(x) = 360 */

    seq_t N[Ndeg + 1];
    seq_t D[Ddeg + 1];
    seq_t Q[Qdeg + 1];
    seq_t a_local[PERIOD] = {0};

    memset(N, 0, sizeof(N));
    memset(D, 0, sizeof(D));

    /* Build N(x) = (x-1)(x^403-1) = x^404 - x^403 - x + 1 */
    N[404] = (seq_t)1;
    N[403] = (seq_t)-1;
    N[1]   = (seq_t)-1;
    N[0]   = (seq_t)1;

    /* Build D(x) = (x^13-1)(x^31-1) = x^44 - x^31 - x^13 + 1 */
    D[44] = (seq_t)1;
    D[31] = (seq_t)-1;
    D[13] = (seq_t)-1;
    D[0]  = (seq_t)1;

    /* Polynomial long division: N/D -> Q, 0 remainder */
    for (int i = Ndeg; i >= Ddeg; --i) {
        seq_t factor = N[i];
        Q[i - Ddeg] = factor;
        int shift = i - Ddeg;
        for (int j = 0; j <= Ddeg; ++j) {
            N[j + shift] -= factor * D[j];
        }
    }

    /* Compute the inverse series a(n) via recurrence */
    a_local[0] = (seq_t)1;           /* constant term */
    for (int n = 1; n < PERIOD; ++n) {
        seq_t sum = (seq_t)0;
        int lim = (n < Qdeg) ? n : Qdeg;
        for (int i = 1; i <= lim; ++i) {
            sum += Q[i] * a_local[n - i];
        }
        a_local[n] = -sum;
    }

    /* Store period values into global static array */
    for (int i = 0; i < PERIOD; ++i) {
        periodArray[i] = a_local[i];
    }
}

seq_t a(idx_t n)
{
    if (!period_initialized) {
        init_seq();
        period_initialized = true;
    }
    return periodArray[(size_t)(n % PERIOD)];
}
