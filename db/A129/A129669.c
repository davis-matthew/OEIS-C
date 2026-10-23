/*
 * A129669  Right truncatable primes in base 3 (written in decimal form).
 * https://oeis.org/A129669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   right truncatable primes in base 3 (written in decimal form)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    /* static storage for the 4 terms */
    static seq_t terms[4] = {0};   /* will be filled on first call */
    static bool initialized = false;

    if (!initialized) {
        idx_t count = 0;
        for (idx_t cand = 2; count < 4; ++cand) {
            seq_t p = (seq_t)cand;
            if (!isPrimeTrial(p))
                continue;
            /* check all right truncations */
            bool ok = true;
            for (seq_t tmp = p; tmp > 0; tmp /= 3) {
                if (!isPrimeTrial(tmp)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                terms[count++] = p;
            }
        }
        initialized = true;
    }

    if (n == 0) /* below OEIS offset, undefined; return 0 */
        return (seq_t)0;
    if (n <= 4)
        return terms[(idx_t)n - 1];
    /* For indices beyond the finite sequence, return 0 (undefined) */
    return (seq_t)0;
}
