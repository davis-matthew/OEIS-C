/*
 * A055039  Numbers of the form 2^(2i+1)*(8j+7).
 * https://oeis.org/A055039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 5461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A055039: numbers of the form 2^(2i+1)*(8j+7). */
/* The implementation simply scans integers and keeps count of those
   whose 2‑adic valuation is odd and whose odd part is congruent to
   7 (mod 8).  This works efficiently for the required limits
   (n ≈ 10 000 gives numbers only about 12 n). */
seq_t a(idx_t n)
{
    idx_t  counter = 0;            /* number of valid terms seen so far   */
    seq_t  cand    = 0;            /* current integer being tested       */

    while (counter < n) {
        /* test next candidate */
        cand++;

        /* compute exponent of 2 (v2) and odd part */
        idx_t v2 = 0;
        seq_t odd = cand;
        /* count trailing zeros */
        while ((odd & (seq_t)1) == (seq_t)0) {
            odd >>= 1;
            v2++;
        }

        /* check conditions: odd exponent of 2 and odd part ≡7 mod 8 */
        if ((v2 & 1) && ((odd & (seq_t)7) == (seq_t)7)) {
            counter++;
        }
    }

    return cand;
}
