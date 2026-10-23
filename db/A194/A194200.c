/*
 * A194200  [sum{(k*e) : 1<=k<=n}], where [ ]=floor, ( )=fractional part.
 * https://oeis.org/A194200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ_{k=1..n} fractional_part(k·e) ) Using the
 *            identity that fractional_part(k·e) = {k·(e-2)}, we iteratively
 *            add the scaled fractional part of (e-2) modulo 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Scale for the fractional part of (e-2). 1 000 000 000 fits in
       seq_t for all builds used in the tests (64‑bit or larger).   */
    const seq_t SCALE = (seq_t)1000000000ULL;          /* 10^9 */
    /* floor((e‑2) × SCALE).  e-2 = 0.718281828459045…          */
    const seq_t ALPHA_FRAC = (seq_t)718281828ULL; /* 0.718281828 × 10^9 */

    seq_t f = 0;          /* current fractional part scaled by SCALE */
    seq_t sum = 0;        /* sum of fractional parts scaled by SCALE  */
    seq_t carry = 0;      /* integer part of the sum (answer)         */

    for (idx_t k = 1; k <= n; ++k)
    {
        f += ALPHA_FRAC;          /* add the fractional part of 1·(e‑2)   */
        if (f >= SCALE)           /* wrap around modulo 1                 */
        {
            f -= SCALE;
        }

        sum += f;                  /* add this fractional part to the sum */
        if (sum >= SCALE)          /* check for carry over               */
        {
            sum -= SCALE;
            ++carry;
        }
    }

    return carry;
}
