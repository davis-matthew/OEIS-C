/*
 * A101678  A modular binomial transform of 10^n.
 * https://oeis.org/A101678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} (-1)^{popcount(n-2k)} ⋅ 𝟙_{(k &
 *            (n-2k))==0} ⋅ 10^k (Σ denotes summation, 𝟙 is the indicator that
 *            equals 1 if the condition is true, otherwise 0; popcount =
 *            number of 1-bits in binary representation)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 19, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;          /* running sum                                      */
    seq_t expo   = 1;          /* 10^k                                            */
    idx_t maxk   = n >> 1;     /* floor(n/2)                                       */
    idx_t k;

    for (k = 0; k <= maxk; ++k) {
        idx_t m = n - (k << 1); /* m = n - 2k                                        */

        /* Binomial(n-k, k) is odd iff (k & (n-2k)) == 0                      */
        if ((k & m) == 0) {
            /* Determine parity of popcount(m) without builtin instructions */
            int parity = 0;
            idx_t t = m;
            while (t) {
                parity ^= 1;
                t &= t - 1;      /* clear lowest set bit */
            }
            if (parity == 0)      /* even popcount → +1   */
                result += expo;
            else                  /* odd popcount  → -1   */
                result -= expo;
        }

        /* Prepare next power of ten: 10^(k+1) */
        expo *= 10;
    }

    return result;
}
