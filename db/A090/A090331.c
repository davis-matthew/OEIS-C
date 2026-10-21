/*
 * A090331  Largest proper divisor of n that is also a prefix of n in binary.
 * https://oeis.org/A090331
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest proper divisor of n that is also a prefix of the
 *            binary representation of n (for n ≥ 2)
 * Range:     u8 n <= 510, u16 n <= 9734, u32 n <= 9734, u64 n <= 9734,
 *            u128 n <= 9734, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* undefined for n < 2, not used */

    /* compute the bit length of n */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    seq_t N = (seq_t)n;
    seq_t best = 1;               /* binary prefix "1" always divides n */

    /* iterate over all proper prefixes (length 1 .. len-1) */
    for (idx_t k = 1; k < len; ++k) {
        idx_t shift = len - k;
        seq_t prefix = (seq_t)(n >> shift);
        if (prefix == 0)
            continue;             /* shouldn't happen for proper prefixes */
        if (N % prefix == 0) {
            if (prefix > best)
                best = prefix;
        }
    }
    return best;
}
