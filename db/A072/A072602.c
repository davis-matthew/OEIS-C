/*
 * A072602  Numbers such that in base 2 the number of 0's is >= the number of
 *          1's.
 * https://oeis.org/A072602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer x > 0 such that in base‑2 the *
 *            number of 0‑bits is at least the number of 1‑bits. Offset:
 *            a(1)=2
 * Range:     u8 n <= 113, u16 n <= 7555, u32 n <= 7555, u64 n <= 7555,
 *            u128 n <= 7555, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation scans candidate integers starting from 2 until
 * the n‑th qualifying number is found.  No dynamic allocation is used,
 * so it works correctly for both fixed‑width uintXX_t types and
 * arbitrary‑precision seq_t classes. */

#include <stdbool.h>
#include <stdlib.h>

/* Count the number of 1‑bits in x.  This works for all seq_t sizes. */
static unsigned popcount_seq(seq_t x)
{
    unsigned cnt = 0;
    while (x)
    {
        x &= x - 1;        /* turn off the lowest set bit */
        ++cnt;
    }
    return cnt;
}

/* Return the bit length of x (position of the highest set bit + 1). */
static unsigned bitlen_seq(seq_t x)
{
    unsigned len = 0;
    while (x)
    {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Check if x has at least as many 0‑bits as 1‑bits in its binary representation. */
static bool qualifies(seq_t x)
{
    if (x == 0) return false;          /* 0 is not part of the sequence */
    unsigned ones = popcount_seq(x);
    unsigned len  = bitlen_seq(x);
    unsigned zeros = len - ones;
    return zeros >= ones;
}

/* ------------------------------------------------------------------ */
/* Public function ----------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        abort();                       /* safeguard for out‑of‑range calls */

    idx_t found = 0;
    seq_t cand = (seq_t)2;              /* sequence starts with 2 */

    while (found < n)
    {
        if (qualifies(cand))
        {
            ++found;
            if (found == n)
                return cand;          /* nth term found */
        }
        ++cand;                        /* next candidate */
    }

    /* The loop always returns; this point is theoretically unreachable. */
    return (seq_t)0;
}
