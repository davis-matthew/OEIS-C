/*
 * A042306  Numerators of continued fraction convergents to sqrt(680).
 * https://oeis.org/A042306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A042306
   Formula: a(n) = 678*a(n-2) - a(n-4)  (n ≥ 4)
   Initial values: a(0)=26, a(1)=339, a(2)=17654, a(3)=229841 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent buffer to avoid recomputation on successive calls. */
    static seq_t *buf = NULL;
    static idx_t cache_cap = 0;

    /* Grow buffer if necessary. */
    if (n + (idx_t)1 > cache_cap) {
        idx_t new_cap = n + (idx_t)1;
        buf = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Base cases. */
    if (n == 0U) {
        buf[0] = (seq_t)26;
        return buf[0];
    }
    if (n == 1U) {
        buf[0] = (seq_t)26;
        buf[1] = (seq_t)339;
        return buf[1];
    }
    if (n == 2U) {
        buf[0] = (seq_t)26;
        buf[1] = (seq_t)339;
        buf[2] = (seq_t)17654;
        return buf[2];
    }
    if (n == 3U) {
        buf[0] = (seq_t)26;
        buf[1] = (seq_t)339;
        buf[2] = (seq_t)17654;
        buf[3] = (seq_t)229841;
        return buf[3];
    }

    /* Initialize first four terms. */
    buf[0] = (seq_t)26;
    buf[1] = (seq_t)339;
    buf[2] = (seq_t)17654;
    buf[3] = (seq_t)229841;

    /* Compute from 4 up to n. */
    for (idx_t i = 4; i <= n; ++i) {
        buf[i] = (seq_t)678 * buf[i - 2] - buf[i - 4];
    }

    return buf[n];
}
