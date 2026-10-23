/*
 * A032545  Integer part of decimal 'base-6 looking' numbers divided by their
 *          actual base-6 values, rounded down.
 * https://oeis.org/A032545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = ⌊A007092(n)/n⌋, where A007092(n) is obtained by
 *            interpreting n in base‑6 as a decimal number (i.e. write the
 *            base‑6 digits of n, then read that string as a base‑10 integer)
 * Range:     u8 n <= 336, u16 n <= 8059, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t  tmp   = n;      /* work on a copy of n                             */
    seq_t  val   = 0;      /* value of A007092(n)                             */
    seq_t  pow10 = 1;      /* current decimal place value (10^i)               */

    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 6);   /* base‑6 digit (0..5)                */
        val += pow10 * d;
        pow10 *= 10;
        tmp  /= 6;
    }
    return val / (seq_t)n;   /* integer division gives floor(val/n)          */
}
