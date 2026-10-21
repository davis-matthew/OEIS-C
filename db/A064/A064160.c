/*
 * A064160  Numbers k such that the digits of k joined to the digits of 2k
 *          contain each of the digits from 1 to 9 once.
 * https://oeis.org/A064160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the nth integer k such that the digits of k and 2*k *
 *            together form a 1‑9 pandigital (each digit 1–9 exactly once). *
 *            The sequence is finite; all 4‑digit k with k ≥ 5000 are tested.
 *            * Implementation uses a one‑time brute‑force search over
 *            k=5000..9999 * and stores the 12 matching values i...
 * Range:     u8 none, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t seq[12];
    static bool init = false;
    static const int FULLMASK = 1022;  /* bits 1..9 set -> (1<<10)-2 */

    if (!init) {
        int cnt = 0;
        for (unsigned k = 5000; k <= 9999; ++k) {
            unsigned x = k;
            unsigned y = k * 2;
            int mask = 0;
            bool ok = true;

            /* digits of k */
            while (x > 0) {
                unsigned d = x % 10;
                if (d == 0 || (mask & (1 << d))) { ok = false; break; }
                mask |= 1 << d;
                x /= 10;
            }
            if (!ok) continue;

            /* digits of 2k */
            while (ok && y > 0) {
                unsigned d = y % 10;
                if (d == 0 || (mask & (1 << d))) { ok = false; break; }
                mask |= 1 << d;
                y /= 10;
            }
            if (!ok) continue;

            if (mask == FULLMASK) {
                seq[cnt++] = (seq_t)k;
            }
        }
        /* seq now contains all 12 terms */
        (void)cnt; /* silence unused warning */
        init = true;
    }

    if (n < 12) {
        return seq[n];
    } else {
        return 0;
    }
}
