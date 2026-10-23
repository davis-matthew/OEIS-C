/*
 * A040621  Continued fraction for sqrt(647).
 * https://oeis.org/A040621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for √647.
   The sequence is a(0)=⌊√647⌋ and for n≥1 it gives the n-th term
   a_n of the simple continued fraction expansion of √647. */
#include <stdlib.h>
#include <stdint.h>

/* initial term a(0) = floor(sqrt(647)) */
static const seq_t _sqrt647_a0 = 25;

/* period of the continued fraction (excluding a(0)) */
static seq_t* _sqrt647_period = NULL;
static size_t _sqrt647_period_len = 0;

/* initialise the period once */
static void _sqrt647_init(void)
{
    if (_sqrt647_period_len != 0) return;          /* already computed */

    const long long D = 647L;                      /* integer D */
    const long long a0 = 25LL;                     /* a(0) */

    long long m = 0LL;
    long long d = 1LL;
    long long a = a0;

    /* growable array for period coefficients */
    size_t cap = 16;
    _sqrt647_period = (seq_t*)malloc(cap * sizeof(seq_t));
    size_t len = 0;

    while (1) {
        /* next values in the continued‑fraction recurrence */
        long long m_next = d * a - m;
        long long d_next = (D - m_next * m_next) / d;
        long long a_next = (a0 + m_next) / d_next;   /* floor division */

        /* store the coefficient (cast to seq_t) */
        if (len == cap) {
            cap <<= 1;
            _sqrt647_period = (seq_t*)realloc(_sqrt647_period, cap * sizeof(seq_t));
        }
        _sqrt647_period[len++] = (seq_t)a_next;

        /* period ends when a_next == 2*a0 */
        if (a_next == 2LL * a0) break;

        /* advance for next iteration */
        m = m_next;
        d = d_next;
        a = a_next;
    }
    _sqrt647_period_len = len;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    /* compute period on first call */
    _sqrt647_init();

    if (n == 0)
        return _sqrt647_a0;
    else {
        /* a(1) is _sqrt647_period[0] */
        size_t idx = (size_t)((n - 1) % _sqrt647_period_len);
        return _sqrt647_period[idx];
    }
}
