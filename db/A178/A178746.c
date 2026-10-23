/*
 * A178746  Binary counter with intermittent bits. Starting at zero the
 *          counter attempts to increment by 1 at each step but each bit in
 *          the counter alternately accepts and rejects requests to toggle.
 * https://oeis.org/A178746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 170, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A178746: Binary counter with intermittent bits (simulation) */

#include <stdbool.h>
#include <string.h>

static const unsigned MAX_BITS = 256;

seq_t a(idx_t n)
{
    seq_t cur = 0;
    bool states[MAX_BITS];
    memset(states, 0, sizeof(states));   /* all bits start in the 'accept' state (false) */

    for (idx_t step = 0; step < n; ++step)
    {
        bool carry = true;
        unsigned i = 0;
        while (carry)
        {
            bool oldbit = ((cur >> i) & 1) != 0;   /* bit value before any toggle */

            if (!states[i])           /* accept: flip the bit */
                cur ^= ((seq_t)1 << i);

            states[i] = !states[i];   /* next toggle for this bit will be the opposite */

            carry = oldbit;           /* carry propagates only if the old bit was 1 */
            ++i;
        }
    }
    return cur;
}
