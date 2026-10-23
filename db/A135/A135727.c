/*
 * A135727  Maximal value in orbit of n under the map A001281(x)=3x-1 if x
 *          odd, x/2 if x even.
 * https://oeis.org/A135727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximal value in orbit of n under map * f(x) = 3x-1 if x
 *            odd, x/2 if x even * * The orbit eventually reaches one of the
 *            three known cycles: * (1,2) → max 2 * (5,7,10,14,20) → max 20 *
 *            (272,136,68,34,17,50,25,74,37,110,55,164,82,41,122,61,182,91) *
 *            → max 272 * Each value encountered before...
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Predefined cycle members */
static const seq_t cycle20[] = {
    (seq_t)5, (seq_t)7, (seq_t)10, (seq_t)14, (seq_t)20
};

static const seq_t cycle272[] = {
    (seq_t)272, (seq_t)136, (seq_t)68,  (seq_t)34,
    (seq_t)17,  (seq_t)50,  (seq_t)25,  (seq_t)74,
    (seq_t)37,  (seq_t)110, (seq_t)55,  (seq_t)164,
    (seq_t)82,  (seq_t)41,  (seq_t)122, (seq_t)61,
    (seq_t)182, (seq_t)91
};

/* Helper to test membership in a small, fixed set */
static inline bool in_set(const seq_t *set, size_t len, seq_t x)
{
    for (size_t i = 0; i < len; ++i)
        if (set[i] == x)
            return true;
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Special case n = 0 → orbit is the constant 0 */
    if (n == 0)
        return (seq_t)0;

    seq_t cur      = n;
    seq_t maxval   = cur;

    for (;;)
    {
        /* Check whether we are already in one of the known cycles */
        if (cur == (seq_t)1 || cur == (seq_t)2)
        {
            if ((seq_t)2 > maxval)
                maxval = (seq_t)2;
            break;
        }
        if (in_set(cycle20, sizeof(cycle20)/sizeof(*cycle20), cur))
        {
            if ((seq_t)20 > maxval)
                maxval = (seq_t)20;
            break;
        }
        if (in_set(cycle272, sizeof(cycle272)/sizeof(*cycle272), cur))
        {
            if ((seq_t)272 > maxval)
                maxval = (seq_t)272;
            break;
        }

        /* Advance to the next element of the orbit */
        if (cur & (seq_t)1)
            cur = cur * (seq_t)3 - (seq_t)1;
        else
            cur >>= 1;   /* shift equivalent to division by 2 for even values */

        if (cur > maxval)
            maxval = cur;
    }

    return maxval;
}
