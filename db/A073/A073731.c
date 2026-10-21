/*
 * A073731  Least k such that A073729(k) = n.
 * https://oeis.org/A073731
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = n mod 10 + signum(n mod 11) * (n - n mod 10)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  for 10 <= n <= 99.  For all other n the function returns 0.  */

seq_t a(idx_t n)
{
    /* Clamp to the valid range of the sequence (10..99).  */
    if (n < 10 || n > 99)
        return 0;

    /* Compute n mod 11 and n mod 10 once in seq_t form.  */
    seq_t sn = (seq_t)n;
    seq_t mod11 = sn % (seq_t)11;
    seq_t mod10 = sn % (seq_t)10;

    /* If mod11 != 0, the value is n itself; otherwise it is n mod 10. */
    if (mod11 != 0)
        return sn;
    else
        return mod10;
}
