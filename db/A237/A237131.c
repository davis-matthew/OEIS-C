/*
 * A237131  Numbers k such that cos(k) = sin(d(1)*d(2)*...*d(q)) where
 *          d(1)d(2)...d(q) denotes the decimal expansion of k and the angles
 *          are in degrees.
 * https://oeis.org/A237131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that cos(k°) = sin(product of the decimal digits of k°).
 * Computation is done by testing each integer sequentially until the n‑th valid
 * term is found. */

#include <stdbool.h>

static bool satisfies(seq_t k)
{
    seq_t prod = 1;
    seq_t tmp = k;

    if (tmp == 0)
        return false;               /* no need to check zero */

    /* compute product of digits modulo 360 */
    while (tmp != 0) {
        int d = (int)(tmp % 10);
        if (d == 0)
            prod = 0;
        else
            prod = (prod * d) % 360;
        tmp /= 10;
    }

    int prod_mod = (int)(prod % 360);
    int target1 = 90 - prod_mod;
    if (target1 < 0)
        target1 += 360;
    int target2 = prod_mod - 90;
    if (target2 < 0)
        target2 += 360;

    int kmod = (int)(k % 360);

    return kmod == target1 || kmod == target2;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 1;

    while (count < n) {
        if (satisfies(k)) {
            count++;
            if (count == n)
                return k;
        }
        k++;
    }

    return 0;   /* unreachable */
}
