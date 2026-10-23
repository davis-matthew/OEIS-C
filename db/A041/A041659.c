/*
 * A041659  Denominators of continued fraction convergents to sqrt(348).
 * https://oeis.org/A041659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041659 – Denominators of continued fraction convergents to sqrt(348)
   Recurrence (for n > 15):
       a(n) = 3134*a(n-8) – a(n-16)
   Initial values (n = 0 … 15) are hard‑coded.
   This implementation evaluates the recurrence iteratively for any n >= 0
   and returns the exact value in seq_t.  No dynamic allocation or
   floating‑point arithmetic is used. */
seq_t a(idx_t n)
{
    /* hard‑coded first 16 terms */
    static const seq_t init[16] = {
        1,   1,   2,   3,   26,  29,  55,  84,
        3079,3163,6242,9405,81482,90887,172369,263256
    };

    if (n < 16)          /* trivial lookup for the first block */
        return init[(idx_t)n];

    /* circular buffer holding the 16 most recent values */
    seq_t buf[16];
    for (int i = 0; i < 16; ++i)
        buf[i] = init[i];

    /* iterate from n = 16 up to the requested n */
    for (idx_t i = 16; i <= n; ++i)
    {
        /* indices wrap around modulo 16 */
        idx_t i8   = (i - 8) % 16;
        idx_t i16  = (i - 16) % 16;
        seq_t val = 3134 * buf[i8] - buf[i16];
        buf[i % 16] = val;
    }

    return buf[n % 16];
}
