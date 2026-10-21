/*
 * A233423  Values of n at which the period of the continued fraction for
 *          sqrt(n) is nondecreasing.
 * https://oeis.org/A233423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer m such that the period of the
 *            simple continued fraction expansion of sqrt(m) is not smaller
 *            than that of any integer less than m. The sequence is built
 *            incrementally by testing successive integers and storing those
 *            that set a new record for the period length
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 <= x < 2^64. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = ((idx_t)1) << 32; /* 2^32 > sqrt(2^64 - 1) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid != 0 && mid > x / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* Length of the continued fraction period of sqrt(n). */
static idx_t period_of_sqrt(idx_t n)
{
    idx_t a0 = isqrt_u64(n);
    if (a0 * a0 == n)        /* perfect square → period 0 */
        return 0;

    idx_t m = 0, d = 1, a = a0;
    idx_t period = 0;

    do {
        /* m = d*a - m; use 128‑bit to avoid overflow */
        unsigned __int128 tmp_m = (unsigned __int128)d * a - m;
        m = (idx_t)tmp_m;

        /* d = (n - m*m) / d; */
        unsigned __int128 tmp_num = (unsigned __int128)n - (unsigned __int128)m * m;
        d = (idx_t)(tmp_num / d);

        /* a = (a0 + m) / d; */
        a = (a0 + m) / d;

        ++period;
    } while (a != 2 * a0);   /* period ends when a == 2*a0 */

    return period;
}

/* Main function for A233423 */
seq_t a(idx_t n)
{
    /* Static mutable state to keep results between calls */
    static seq_t *qual = NULL;       /* array of qualifying integers           */
    static size_t qual_len = 0;      /* number of stored elements             */
    static size_t qual_cap = 0;      /* allocated capacity of the array       */
    static idx_t current_max_period = 0;   /* best period found so far             */
    static idx_t last_checked = 1;  /* last integer that was processed       */

    /* Initialise only once */
    if (qual_len == 0) {
        qual_cap = 4;
        qual = (seq_t *)malloc(qual_cap * sizeof(seq_t));
        qual[0] = (seq_t)1;             /* a(1) = 1 */
        qual_len = 1;
        current_max_period = 0;         /* period of 1 is 0 */
        last_checked = 1;
    }

    /* Generate additional elements until we have at least n of them */
    while (qual_len < n) {
        idx_t candidate = ++last_checked;          /* next integer to test   */
        idx_t period   = period_of_sqrt(candidate);

        if (period >= current_max_period) {        /* record a new record   */
            if (qual_len == qual_cap) {            /* expand storage      */
                size_t new_cap = qual_cap << 1;
                if (new_cap == 0) new_cap = 8;
                seq_t *tmp = (seq_t *)realloc(qual,
                                               new_cap * sizeof(seq_t));
                if (!tmp) abort();                 /* allocation failed   */
                qual = tmp;
                qual_cap = new_cap;
            }
            qual[qual_len++] = (seq_t)candidate;
            current_max_period = period;
        }
    }

    return qual[n - 1];          /* a is 1‑indexed */
}
