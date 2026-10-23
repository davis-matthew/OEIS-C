/*
 * A042878  Numerators of continued fraction convergents to sqrt(971).
 * https://oeis.org/A042878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 24959613572660*a(n-22) - a(n-44) for n > 43 Initial terms
 *            (n, a(n)): (0,31) (1,187) (2,779) (3,966) (4,1745) (5,2711)
 *            (6,7167) (7,38546) (8,122805) (9,284156) (10,3532677)
 *            (11,109797143) (12,1321098393) (13,2751993929) (14,9577080180)
 *            (15,50637394829) (16,110851869838) (17,161489264667) (1...
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 13, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Maximum n expected in the tests; adjust if needed */
#define MAX_N 10000

seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t cache[MAX_N + 1];
    static idx_t cached = 0;
    static bool initialized = false;

    /* Constant multiplier in the recurrence */
    static const seq_t K = (seq_t)24959613572660ULL;

    /* Compute first 44 terms via continued‑fraction expansion of sqrt(971) */
    if (!initialized)
    {
        const uint64_t D = 971ULL;
        const uint64_t a0 = 31ULL;          /* floor(sqrt(971)) */

        uint64_t m = 0, d = 1, a_cf = a0;

        /* p_{-2} = 0, p_{-1} = 1 */
        seq_t p_prev2 = 0;
        seq_t p_prev1 = 1;

        /* Compute terms 0..43 */
        for (idx_t i = 0; i < 44; ++i)
        {
            seq_t p = a_cf * p_prev1 + p_prev2;
            cache[i] = p;

            /* Next CF component */
            uint64_t m_next = d * a_cf - m;
            uint64_t d_next = (D - m_next * m_next) / d;
            uint64_t a_next = (a0 + m_next) / d_next;

            /* Shift for next iteration */
            p_prev2 = p_prev1;
            p_prev1 = p;
            m = m_next;
            d = d_next;
            a_cf = a_next;
        }

        cached = 44;           /* terms 0..43 have been computed */
        initialized = true;
    }

    /* Compute additional terms if needed */
    if (n >= cached)
    {
        for (idx_t i = cached; i <= n; ++i)
        {
            cache[i] = K * cache[i - 22] - cache[i - 44];
        }
        cached = n + 1;
    }

    return cache[n];
}
