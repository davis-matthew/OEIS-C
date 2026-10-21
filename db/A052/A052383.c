/*
 * A052383  Numbers without 1 as a digit.
 * https://oeis.org/A052383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = replace each non‑zero digit d of (n-1) written in base‑9
 *            by d+1 (digits 1–8 become 2–9, zeros stay zero). This yields the
 *            nth integer (starting at n=1) that contains no decimal digit 1
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based.  Compute n‑1 in base‑9 and build the result in decimal. */
    idx_t m = n - 1;            /* still fits in idx_t (unsigned) */
    seq_t result = 0;
    seq_t pow10  = 1;           /* 10^position */

    while (m > 0) {
        idx_t d = m % 9;        /* least‑significant base‑9 digit */
        m /= 9;
        seq_t mapped = (seq_t)d;
        if (mapped != 0)        /* map 1–8 to 2–9 */
            mapped = mapped + 1;
        result += pow10 * mapped;   /* add the decimal digit */
        pow10 *= 10;                  /* shift to next decimal place */
    }
    /* When m == 0, result is already correct (0 for a(1)). */
    return result;
}
