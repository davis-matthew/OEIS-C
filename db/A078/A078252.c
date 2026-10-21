/*
 * A078252  Smallest n-digit multiple of n in which the even-numbered digits
 *          are all equal and the odd-numbered digits are all equal, or 0 if
 *          no such number exists.
 * https://oeis.org/A078252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the smallest n‑digit integer whose even‑position digits are all equal and
   whose odd‑position digits are all equal, and which is a multiple of n.
   The function returns 0 if no such integer exists. It works for any
   size of seq_t (uint8_t … unsigned __int128 or arbitrary precision). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial n == 0 (not part of OEIS, but avoid divide by zero) */
    if (n == 0)
        return 0;

    /* Loop over all allowed digit patterns.
       O = odd‑position digit (must be 1–9 to avoid leading zero).
       E = even‑position digit (can be 0–9). */
    for (int O = 1; O <= 9; ++O) {
        for (int E = 0; E <= 9; ++E) {
            idx_t mod = 0;          /* current remainder modulo n */
            for (idx_t pos = 1; pos <= n; ++pos) {
                int d = (pos & 1) ? O : E;   /* odd pos: O, even pos: E */
                mod = ((mod * 10) + d) % n;
            }
            if (mod == 0) { /* pattern is a valid candidate; it's minimal by construction */
                seq_t val = 0;
                for (idx_t pos = 1; pos <= n; ++pos) {
                    int d = (pos & 1) ? O : E;
                    val = val * 10 + d;
                }
                return val;
            }
        }
    }
    /* No pattern found */
    return 0;
}
