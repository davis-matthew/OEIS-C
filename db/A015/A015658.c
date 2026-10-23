/*
 * A015658  Inverse of 1649th cyclotomic polynomial.
 * https://oeis.org/A015658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   Inverse of the 1649th cyclotomic polynomial Sequence is periodic
 *            with period 1649
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define PERIOD 1649
#define DEG_P 1650
#define DEG_Q 114
#define DEG_PHI 1536

static bool _init_done = false;
static seq_t _phi_coeff[DEG_PHI + 1];
static seq_t _seq_vals[PERIOD];

/* Build Φ_1649(x) and generate the periodic sequence */
static void _init_seq(void)
{
    seq_t P[DEG_P + 1];
    seq_t Q[DEG_Q + 1];

    /* initialise to zero */
    for (size_t i = 0; i <= DEG_P; ++i) P[i] = 0;
    for (size_t i = 0; i <= DEG_Q; ++i) Q[i] = 0;

    /* P(x) = (x-1) * (x^1649-1) = x^1650 - x^1649 - x + 1 */
    P[0]    = 1;
    P[1]    = -1;
    P[1649] = -1;
    P[1650] = 1;

    /* Q(x) = (x^17-1) * (x^97-1) = x^114 - x^97 - x^17 + 1 */
    Q[0]   = 1;
    Q[17]  = -1;
    Q[97]  = -1;
    Q[114] = 1;

    /* long division P / Q -> Φ_1649(x) stored in _phi_coeff */
    for (size_t i = 0; i <= DEG_PHI; ++i) _phi_coeff[i] = 0;
    for (long long i = DEG_P; i >= (long long)DEG_Q; --i) {
        seq_t qcoeff = P[(size_t)i];
        size_t shift = (size_t)(i - DEG_Q);
        _phi_coeff[shift] = qcoeff;
        for (size_t j = 0; j <= DEG_Q; ++j) {
            P[shift + j] -= qcoeff * Q[j];
        }
    }

    /* generate a(n) for n = 0 .. PERIOD-1 using the recurrence
       Φ(x) * A(x) = 1  (formal power series) */
    _seq_vals[0] = 1;
    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t max_k = (n <= (size_t)DEG_PHI) ? n : (size_t)DEG_PHI;
        for (size_t k = 1; k <= max_k; ++k) {
            sum += _phi_coeff[k] * _seq_vals[n - k];
        }
        _seq_vals[n] = -sum;
    }

    _init_done = true;
}

/* External function for the OEIS sequence */
seq_t a(idx_t n)
{
    if (!_init_done) _init_seq();
    size_t idx = (size_t)(n % PERIOD);
    return _seq_vals[idx];
}
