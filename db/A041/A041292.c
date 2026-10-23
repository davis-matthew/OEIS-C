/*
 * A041292  Numerators of continued fraction convergents to sqrt(159).
 * https://oeis.org/A041292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of sqrt(159)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* constants for sqrt(159) */
    const idx_t D      = 159;
    const idx_t a0     = 12;            /* floor(sqrt(159)) */

    /* static state for memoisation */
    static seq_t *num_cache     = NULL;  /* array of numerators p_k */
    static idx_t  num_len       = 0;     /* number of cached numerators */
    static idx_t  num_cap       = 0;     /* capacity of num_cache */

    static idx_t *m_cache      = NULL;  /* array of m_k */
    static idx_t *d_cache      = NULL;  /* array of d_k */
    static idx_t  mc_len       = 0;
    static idx_t  mc_cap       = 0;

    static idx_t *a_cache      = NULL;  /* array of a_k */
    static idx_t  ac_len       = 0;
    static idx_t  ac_cap       = 0;

    /* extend caches if necessary */
    if (n + 1 > num_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp_num = (seq_t *)realloc(num_cache, new_cap * sizeof(seq_t));
        if (tmp_num) num_cache = tmp_num;
        num_cap = new_cap;

        idx_t *tmp_m   = (idx_t *)realloc(m_cache, new_cap * sizeof(idx_t));
        if (tmp_m) m_cache = tmp_m;
        mc_cap = new_cap;

        idx_t *tmp_d   = (idx_t *)realloc(d_cache, new_cap * sizeof(idx_t));
        if (tmp_d) d_cache = tmp_d;
        mc_cap = new_cap;

        idx_t *tmp_a   = (idx_t *)realloc(a_cache, new_cap * sizeof(idx_t));
        if (tmp_a) a_cache = tmp_a;
        ac_cap = new_cap;
    }

    /* initialise internal state for the recurrence if we start from scratch */
    static seq_t prev1 = 1;   /* p_{-1} */
    static seq_t prev2 = 0;   /* p_{-2} */
    if (num_len == 0) {
        prev1 = 1;
        prev2 = 0;
    }

    /* extend numerator and CF state up to index n */
    for (idx_t k = num_len; k <= n; ++k) {
        idx_t ak;          /* partial quotient a_k */
        idx_t mk;          /* m_k   */
        idx_t dk;          /* d_k   */

        if (k == 0) {
            /* first term from the integer part of sqrt(159) */
            ak = a0;
            mk = 0;
            dk = 1;
        } else {
            /* use previous CF values */
            idx_t prev_m = m_cache[k - 1];
            idx_t prev_d = d_cache[k - 1];
            idx_t prev_a = a_cache[k - 1];

            mk = prev_d * prev_a - prev_m;
            dk = (D - mk * mk) / prev_d;
            ak = (a0 + mk) / dk;
        }

        /* store partial quotient and CF terms */
        a_cache[k] = ak;
        m_cache[k] = mk;
        d_cache[k] = dk;

        /* compute numerator p_k */
        seq_t pk = (seq_t)ak * prev1 + prev2;
        num_cache[k] = pk;

        /* shift recurrence states for next step */
        prev2 = prev1;
        prev1 = pk;
    }

    return num_cache[n];
}
