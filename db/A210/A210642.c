/*
 * A210642  a(n) = least integer m > 1 such that k! == n! (mod m) for no 0 < k
 *          < n.
 * https://oeis.org/A210642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer m > 1 such that k! == n! (mod m) for no 0 <
 *            k < n
 * Range:     u8 n <= 203, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* Undefined for n = 0, but not needed */

    /* Start candidates from max(2, n).  Candidates m ≤ n-1 are impossible
       because k = m would give n! ≡ k! ≡ 0 (mod m).  The known exceptional
       values a(4)=4 and a(6)=9 are found with this search. */
    idx_t m_start = (n >= 2) ? n : 2;

    for (idx_t m = m_start; ; ++m)
    {
        /* Compute n! mod m */
        unsigned long long nf = 1ULL;
        for (idx_t k = 1; k <= n; ++k)
            nf = (nf * k) % m;

        /* Check all k < n */
        unsigned long long f = 1ULL;
        bool bad = false;
        for (idx_t k = 1; k < n; ++k)
        {
            f = (f * k) % m;
            if (f == nf) { bad = true; break; }
        }

        if (!bad)
            return (seq_t)m;
    }

    /* Unreachable: a(n) always exists. */
}
