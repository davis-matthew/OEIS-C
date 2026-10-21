/*
 * A213258  Positive integers that are not in A213257.
 * https://oeis.org/A213258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A213258.
 * a(n) is the nth positive integer whose 2‑adic valuation is
 * congruent to 2 modulo 3, i.e. numbers of the form 2^(3k+2)*s
 * with s odd.  The implementation searches for such numbers.
 */
seq_t a(idx_t n)
{
    seq_t m = (seq_t)4;          /* first candidate (4 = 2^2) */
    idx_t count = 0;

    while (1) {
        if ((m & (seq_t)1) == 0) {          /* m is even – only those can
                                               have valuation ≥ 2 */
            seq_t tmp = m;
            unsigned int exp = 0;

            /* count trailing zeros: the 2‑adic valuation of m */
            while ((tmp & (seq_t)1) == 0) {
                tmp >>= 1;
                exp++;
            }

            if (exp % 3 == 2) {              /* exponent ≡ 2 (mod 3) */
                ++count;
                if (count == n)
                    return m;
            }
        }
        ++m;
    }
}
