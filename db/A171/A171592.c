/*
 * A171592  a(n) = 0+1+2+...+n in lunar arithmetic in base 6 written in base
 *          10.
 * https://oeis.org/A171592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{k>=0} min(5, floor(n / 6^k)) * 6^k
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the sum 0+1+2+...+n performed in lunar arithmetic base 6;
   the result is returned in base 10. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    /* Process each base‑6 digit position.  pow holds 6^k as an idx_t. */
    idx_t pow = 1;

    while (pow <= n) {
        /* Highest digit seen at this position is min(5, floor(n / 6^k)) */
        idx_t q = n / pow;
        seq_t digit = (seq_t)q;
        if (digit > 5) digit = 5;

        result += digit * (seq_t)pow;

        /* Avoid overflow when computing the next power of 6. */
        if (pow > n / 6) break;
        pow *= 6;
    }

    return result;
}
