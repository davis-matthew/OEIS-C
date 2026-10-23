/*
 * A190640  Numbers whose base-3 expansion ends in 2 and does not contain any
 *          1's.
 * https://oeis.org/A190640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2 * (1 + Σ_{i≥1} [bit(i-1) of (n-1)] * 3^i)
 * Range:     u8 n <= 16, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A190640 is the set of numbers whose base‑3 expansion
   ends in 2 and contains no digit 1.  The formula above
   converts the index n (starting at 1) to the corresponding
   number by interpreting the binary digits of n−1 as a
   selection of powers of three. (e₀ is always 1, giving the
   trailing 2.)  */

/* External linkage as required by the build system. */
seq_t a(idx_t n)
{
    /* sum holds the value 1 + Σ e_i * 3^i, where e₀=1. */
    seq_t sum   = 1;          /* contribution of the least significant digit (2). */
    seq_t pow3  = 3;          /* 3^1 for the first higher digit. */
    idx_t k = n - 1;          /* index encoded in binary; bit i corresponds to e_{i+1}. */

    while (k) {
        if (k & 1) {          /* if current bit is set, include the corresponding
                                 power of three in sum. */
            sum += pow3;
        }
        pow3 *= 3;            /* advance to next power of three. */
        k >>= 1;              /* process next bit. */
    }
    return sum * 2;           /* multiply by 2 to account for the trailing digit 2. */
}
