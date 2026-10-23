/*
 * A042042  Numerators of continued fraction convergents to sqrt(545).
 * https://oeis.org/A042042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042042: Numerators of continued‑fraction convergents to sqrt(545)
   Recurrence: a(n) = 3922*a(n-6) - a(n-12),  n ≥ 12
   Initial values (n = 0 … 11):
       23, 47, 70, 607, 677, 1961,
       90883, 183727, 274610, 2380607,
       2655217, 7691041
   The function returns a(n) for any n ≥ 0, using only a small
   12‑element circular buffer.  It works for any unsigned seq_t
   (8‑, 16‑, 32‑, 64‑, or 128‑bit, or arbitrary‑precision). */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values (cast to seq_t for arbitrary‑precision types). */
    static const seq_t init[12] = {
        (seq_t)23,      (seq_t)47,      (seq_t)70,      (seq_t)607,
        (seq_t)677,     (seq_t)1961,    (seq_t)90883,  (seq_t)183727,
        (seq_t)274610,  (seq_t)2380607, (seq_t)2655217, (seq_t)7691041
    };

    if (n < 12)
        return init[(size_t)n];

    /* Circular buffer of 12 consecutive terms. */
    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* Compute each term from n = 12 up to the requested n. */
    for (idx_t i = 12; i <= n; ++i)
    {
        size_t cur   = (size_t)i % 12;
        size_t six   = (size_t)(i - 6) % 12;
        size_t t12   = (size_t)(i - 12) % 12;
        buf[cur] = buf[six] * (seq_t)3922 - buf[t12];
    }

    return buf[(size_t)n % 12];
}
