/*
 * A212314  Numbers m such that B(m^3) = 3*B(m), where B(m) is the binary
 *          weight of m (A000120).
 * https://oeis.org/A212314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers m such that B(m^3) = 3*B(m), where B(x) is
 *            popcount(x) (A000120)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 1223, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include utilities for dynamic allocation */
#include "../utils/ensure_capacity.h"

/* Helper: popcount for arbitrary-width seq_t */
static inline unsigned popcount_seq_t(seq_t x)
{
    unsigned c = 0;
    while (x)
    {
        c += (unsigned)(x & 1);
        x >>= 1;
    }
    return c;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static state for memoized generation */
    static seq_t *terms = NULL;
    static size_t capacity = 0;
    static size_t len = 0;
    static idx_t next_candidate = 0;   /* next integer to test */

    if (n == 0) /* undefined in OEIS, but guard against caller misuse */
        return 0;

    /* generate terms until we have at least n entries */
    while (len < n)
    {
        seq_t candidate = 0;

        /* find next term */
        for (;; ++next_candidate)
        {
            seq_t sm = (seq_t)next_candidate;
            unsigned bc = popcount_seq_t(sm);

            seq_t m2 = sm * sm;
            seq_t m3 = m2 * sm;
            unsigned bc3 = popcount_seq_t(m3);

            if (bc3 == 3 * bc)
            {
                candidate = sm;
                ++next_candidate;   /* start next search from next integer */
                break;
            }
        }

        /* ensure we have space in the array */
        if (len >= capacity)
        {
            size_t newcap = capacity ? capacity << 1 : 16;
            ensure_capacity(len + 1, &capacity, (void **)&terms, sizeof(seq_t));
        }

        terms[len++] = candidate;
    }

    return terms[n - 1];
}
