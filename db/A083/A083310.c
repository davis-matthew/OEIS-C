/*
 * A083310  Least number greater than n that contains all the decimal digits
 *          of n.
 * https://oeis.org/A083310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the least integer > n that contains all decimal digits appearing in n
   (OEIS A083310).  The function works for all supported integer types
   (fixed‑width or arbitrary‑precision). */

static unsigned digit_mask(seq_t x)
/* Return a 16‑bit mask whose bit d is set iff digit d occurs in x. */
{
    unsigned mask = 0;
    if (x == 0)          /* special case: 0 contains only the digit 0 */
        return 1U << 0;

    while (x != 0) {
        /* x % 10 and x / 10 are defined for all seq_t types */
        unsigned d = (unsigned)(x % 10);
        mask |= 1U << d;
        x /= 10;
    }
    return mask;
}

seq_t a(idx_t n)
{
    const unsigned need = digit_mask((seq_t)n);     /* digits that must appear */
    seq_t cand = (seq_t)n + 1;                     /* start searching above n */

    while ( (digit_mask(cand) & need) != need )
        ++cand;  /* increment until we have all required digits */

    return cand;
}
