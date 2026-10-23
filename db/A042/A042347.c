/*
 * A042347  Denominators of continued fraction convergents to sqrt(700).
 * https://oeis.org/A042347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominators of continued fraction convergents to
 *            sqrt(700). * (Recurrence: a(n) = 16386302*a(n-16) - a(n-32) for
 *            n>31.) * The sequence is computed via the standard
 *            continued‑fraction algorithm * for sqrt(700), which produces the
 *            same terms as the recurrence satisfaction
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types are provided by the build. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *den = NULL;     /* array of denominators q_n            */
    static size_t den_size = 0;   /* number of terms stored                */
    static size_t den_cap = 0;    /* current capacity of the array         */

    /* Continued‑fraction state variables. */
    static unsigned long long m = 0;
    static unsigned long long d = 1;
    static const unsigned long long a0 = 26ULL;    /* floor(sqrt(700)) = 26       */
    static unsigned long long a_cur = 26ULL;      /* current partial quotient   */
    static seq_t q_prev2 = 1;   /* q_{-2}                */
    static seq_t q_prev1 = 0;   /* q_{-1}                */

    /* Initialise cache on first call. */
    if (den == NULL) {
        den_cap = 64;
        den = (seq_t*)malloc(den_cap * sizeof(seq_t));
        if (den == NULL) {
            /* Memory allocation failed – return 0 as safe default. */
            return (seq_t)0;
        }
    }

    /* Generate terms until the requested index is available. */
    while (den_size <= n) {
        /* Compute next denominator q. */
        seq_t q = a_cur * q_prev1 + q_prev2;

        /* Extend cache array if needed. */
        if (den_size >= den_cap) {
            size_t new_cap = den_cap * 2;
            seq_t *tmp = (seq_t*)realloc(den, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Memory allocation failed – return 0 to avoid UB. */
                return (seq_t)0;
            }
            den = tmp;
            den_cap = new_cap;
        }

        /* Store the computed denominator. */
        den[den_size] = q;

        /* Shift recurrence variables for the next iteration. */
        seq_t prev = q_prev1;
        q_prev1 = q;
        q_prev2 = prev;

        /* Prepare the next partial quotient a (used for the following term). */
        unsigned long long m_next = d * a_cur - m;
        unsigned long long d_next = (700ULL - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;
        m = m_next;
        d = d_next;
        a_cur = a_next;

        den_size++;
    }

    return den[n];
}
