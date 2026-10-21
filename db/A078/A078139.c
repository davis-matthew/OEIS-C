/*
 * A078139  Primes which cannot be written as sum of squares>1.
 * https://oeis.org/A078139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   primes that cannot be expressed as a sum of squares of integers
 *            ≥ 2. The sequence is finite: 2, 3, 5, 7, 11, 19, 23
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    static seq_t seq[20];
    static size_t seq_len = 0;
    static bool initialized = false;

    if (!initialized)
    {
        const idx_t LIMIT = 100;   /* enough to capture all terms (≥ 23) */

        for (idx_t cand = 2; cand <= LIMIT; ++cand)
        {
            if (!isPrimeTrial((seq_t)cand))
                continue;

            /* dynamic‑programming: is cand a sum of squares ≥ 4? */
            bool reachable[101] = { false };
            reachable[0] = true;

            for (idx_t sum = 0; sum <= cand; ++sum)
            {
                if (!reachable[sum])
                    continue;

                for (uint64_t s = 2; ; ++s)
                {
                    uint64_t sq = s * s;      /* s² ≥ 4 */
                    if (sq > (uint64_t)cand)
                        break;

                    if (sum + sq <= cand)
                        reachable[(size_t)(sum + sq)] = true;
                }
            }

            if (!reachable[cand])
            {
                seq[seq_len++] = (seq_t)cand;
                if (seq_len >= sizeof(seq) / sizeof(seq[0]))
                    break;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > seq_len)
        return 0;          /* undefined beyond the final term */

    return seq[n - 1];
}
