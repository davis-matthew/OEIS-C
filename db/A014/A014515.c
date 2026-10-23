/*
 * A014515  Inverse of 506th cyclotomic polynomial.
 * https://oeis.org/A014515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in ((x+1)*(x^10 - x^9 + x^8 - ...
 *            + 1)*(x^22 - x^21 + ... + 1))/(1 + x^253)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence has period 506. */

#include <stdbool.h>

static bool _init = false;
static seq_t _coeff[34];

static void _init_coeff(void)
{
    /* a(x) = (x^10 - x^9 + ... + 1) , 11 terms, alternating signs */
    seq_t a[11];
    for (idx_t i = 0; i < 11; ++i) {
        a[i] = (i % 2 == 0) ? 1 : -1;
    }

    /* b(x) = (x^22 - x^21 + ... + 1) , 23 terms, alternating signs */
    seq_t b[23];
    for (idx_t i = 0; i < 23; ++i) {
        b[i] = (i % 2 == 0) ? 1 : -1;
    }

    /* d(x) = a(x) * b(x), 33 terms (indices 0..32) */
    seq_t d[33] = {0};
    for (idx_t i = 0; i < 11; ++i) {
        for (idx_t j = 0; j < 23; ++j) {
            d[i + j] += a[i] * b[j];
        }
    }

    /* c(x) = (x+1)*d(x), 34 terms (indices 0..33) */
    for (idx_t k = 0; k < 33; ++k) {
        _coeff[k] = d[k] + (k > 0 ? d[k - 1] : 0);
    }
    _coeff[33] = d[32];
}

seq_t a(idx_t n)
{
    if (!_init) {
        _init_coeff();
        _init = true;
    }

    const idx_t period = 506;
    idx_t m = n % period;          /* periodicity */
    seq_t result = 0;

    if (m <= 33) {
        result += _coeff[m];
    }
    if (m >= 253) {
        idx_t t = m - 253;
        if (t <= 33) {
            result -= _coeff[t];
        }
    }
    return result;
}
