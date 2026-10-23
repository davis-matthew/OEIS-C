/*
 * A136804  Nonsquares mod 257.
 * https://oeis.org/A136804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, easy, nonn
 * Formula:   a(n) is the n-th primitive root modulo 257 in increasing order
 * Range:     u8 none, u16 n <= 128, u32 n <= 128, u64 n <= 128, u128 n <= 128,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                (Sequences A136804 – Nonsquares mod 257) */
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* There are exactly 128 primitive roots modulo 257. */
    static seq_t roots[128];
    static bool ready = false;

    if (!ready) {
        idx_t cnt = 0;
        for (idx_t cand = 2; cand < 257 && cnt < 128; ++cand) {
            /* x is a primitive root iff x^(256/2) ≠ 1 (mod 257).
               Here 256 = 2^8, so we test x^128 mod 257.   */
            seq_t r = mod_pow((seq_t)cand, 128, (seq_t)257);
            if (r != 1) {
                roots[cnt++] = (seq_t)cand;
            }
        }
        ready = true;
    }

    if (n == 0 || n > 128) {
        /* Undefined beyond the 128 primitive roots – return 0. */
        return 0;
    }
    return roots[n - 1];
}
