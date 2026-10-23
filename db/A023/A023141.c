/*
 * A023141  Number of cycles of function f(x) = 9x mod n.
 * https://oeis.org/A023141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|m} phi(d)/ord(9,d), where m = n with 3's removed
 * Range:     u8 n <= 867, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"
#include "../utils/mod_pow.h"

static seq_t order9(idx_t d)
{
    if (d == 1)
        return (seq_t)1;

    seq_t mod   = (seq_t)d;
    seq_t base  = (seq_t)(9 % d);
    seq_t phi_d = calc_phi((seq_t)d);

    /* Gather all divisors of phi_d */
    seq_t divs[128];
    size_t cnt = 0;
    for (idx_t i = 1; (idx_t)i * (idx_t)i <= phi_d; i++) {
        if (phi_d % (idx_t)i == 0) {
            divs[cnt++] = (seq_t)i;
            if ((idx_t)i != phi_d / (idx_t)i)
                divs[cnt++] = (seq_t)(phi_d / (idx_t)i);
        }
    }

    /* Simple insertion sort (divisors are few). */
    for (size_t i = 0; i < cnt; i++) {
        for (size_t j = i + 1; j < cnt; j++) {
            if (divs[j] < divs[i]) {
                seq_t tmp = divs[i];
                divs[i]   = divs[j];
                divs[j]   = tmp;
            }
        }
    }

    /* Test each divisor for order */
    for (size_t k = 0; k < cnt; k++) {
        if (mod_pow(base, (uint64_t)divs[k], mod) == (seq_t)1)
            return divs[k];
    }
    return phi_d;   /* fallback, should not happen */
}

seq_t a(idx_t n)
{
    /* Remove all factors of 3 from n */
    idx_t m = n;
    while (m % 3 == 0)
        m /= 3;

    /* Gather all divisors of m */
    seq_t sum    = (seq_t)0;
    idx_t divs[128];
    size_t cnt   = 0;
    for (idx_t i = 1; (idx_t)i * (idx_t)i <= m; i++) {
        if (m % i == 0) {
            divs[cnt++] = i;
            if (i != m / i)
                divs[cnt++] = m / i;
        }
    }

    for (size_t k = 0; k < cnt; k++) {
        idx_t d     = divs[k];
        seq_t phi_d = calc_phi((seq_t)d);
        seq_t ord   = order9(d);
        sum += phi_d / ord;
    }

    return sum;
}
