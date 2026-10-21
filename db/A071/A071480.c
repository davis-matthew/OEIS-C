/*
 * A071480  Sprague-Grundy values for octal games .173 and .1731.
 * https://oeis.org/A071480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sprague‑Grundy values for octal game .173.
   The sequence becomes periodic with period 40 from n = 21 on.
   The values are computed by dynamic programming using the
   octal description:
   digit i (starting at 1) is an octal number; its binary bits
   indicate allowed numbers of heaps to leave after deleting i stones:
   bit 0 (value 1)  → leave 0 heaps (possible only if all stones are removed),
   bit 1 (value 2)  → leave 1 heap (no split),
   bit 2 (value 4)  → leave 2 heaps (split the remaining stones). */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precompute Grundy numbers for heap sizes 0…60.
       sg[0] is the value of an empty heap and is 0 by definition. */
    static seq_t sg[61];
    static bool ready = false;

    if (!ready) {
        /* Octal digits for .173 : 1 → 001, 7 → 111, 3 → 011 */
        const unsigned char dig[3] = {1, 7, 3};

        sg[0] = 0;     /* empty heap */

        for (idx_t N = 1; N <= 60; ++N) {
            bool seen[512] = {false};

            for (idx_t k = 1; k <= 3; ++k) {
                if (k > N)
                    continue;       /* cannot remove more stones than in the heap */

                unsigned char d = dig[k - 1];
                idx_t rem = N - k;

                /* bit 0 : leave 0 heaps (only if all stones are removed) */
                if (d & 1u) {
                    if (rem == 0)
                        seen[(size_t)sg[0]] = true;
                }

                /* bit 1 : leave 1 heap (no split) */
                if (d & 2u) {
                    if (rem >= 1)
                        seen[(size_t)sg[rem]] = true;
                }

                /* bit 2 : leave 2 heaps (split the remainder) */
                if (d & 4u) {
                    if (rem >= 2) {
                        for (idx_t a = 1; a <= rem - 1; ++a) {
                            seq_t g = sg[a] ^ sg[rem - a];
                            seen[(size_t)g] = true;
                        }
                    }
                }
            }

            /* mex of seen values */
            seq_t mex = 0;
            while (seen[(size_t)mex])
                ++mex;
            sg[N] = mex;
        }

        ready = true;
    }

    if (n <= 60)
        return sg[(size_t)n];

    /* Periodic continuation: period 40 from n = 21 onward. */
    idx_t r = (n - 21) % 40;
    return sg[(size_t)(21 + r)];
}
