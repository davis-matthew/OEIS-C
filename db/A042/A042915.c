/*
 * A042915  Denominators of continued fraction convergents to sqrt(989).
 * https://oeis.org/A042915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of the continued
 *            fraction for sqrt(989)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 23, u64 n <= 39, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const uint64_t D = 989;      /* D for sqrt(D) */
static const uint64_t A0 = 31;      /* floor(sqrt(989)) */

/* Computes a(n).  The static state is used to memoise previously computed terms. */
seq_t a(idx_t n)
{
    /* Static storage for denominators */
    static seq_t *denoms = NULL;
    static size_t denoms_len = 0;   /* number of terms stored (denoms[0] is term 0) */
    static size_t denoms_cap = 0;

    /* State variables for continued fraction generation */
    static uint64_t m = 0;          /* current m value */
    static uint64_t d = 1;          /* current d value */
    static uint64_t a_cf = 0;       /* current a (for the next term) */

    /* Initialise on the first call */
    if (denoms == NULL) {
        denoms_cap = 256;
        denoms = (seq_t*)malloc(denoms_cap * sizeof(seq_t));
        if (denoms == NULL) return 0;   /* allocation failure */

        /* Term 0: k0 = 1 */
        denoms[0] = (seq_t)1;
        denoms_len = 1;                 /* we have stored term 0 */

        /* Prepare state for term 1 (a1) */
        uint64_t m_next = d * A0 - m;      /* m1 = d0*A0 - m0 */
        uint64_t d_next = (D - m_next * m_next) / d;   /* d1 = (D - m1^2)/d0 */
        a_cf = (A0 + m_next) / d_next;    /* a1 = floor((A0 + m1)/d1) */

        m = m_next;   d = d_next;   /* state set to (m1, d1) for the next iteration */
    }

    /* Extend the memoised list up to n if necessary */
    while (denoms_len <= (size_t)n) {
        /* Compute denominator for current index (denoms_len) using a_cf */
        seq_t denom;
        if (denoms_len == 1) {
            /* k_1 = a1*k_0 + k_-1 (k_-1 = 0) */
            denom = (seq_t)a_cf * denoms[0];
        } else {
            /* k_k = a_k * k_{k-1} + k_{k-2} */
            denom = (seq_t)a_cf * denoms[denoms_len - 1] + denoms[denoms_len - 2];
        }

        /* Ensure capacity for the new term */
        if (denoms_len + 1 > denoms_cap) {
            size_t new_cap = denoms_cap * 2;
            seq_t *tmp = (seq_t*)realloc(denoms, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;   /* allocation failure */
            denoms = tmp;
            denoms_cap = new_cap;
        }

        denoms[denoms_len] = denom;
        denoms_len++;

        /* Advance continued fraction state to next a value */
        uint64_t m_next = d * a_cf - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        a_cf = (A0 + m_next) / d_next;

        m = m_next;
        d = d_next;
    }

    return denoms[(size_t)n];
}
