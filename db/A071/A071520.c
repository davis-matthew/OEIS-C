/*
 * A071520  Number of 5-smooth numbers (A051037) <= n.
 * https://oeis.org/A071520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of 5‑smooth numbers (of the form 2^x·3^y·5^z) that
 *            are ≤ n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;

    unsigned __int128 pow5 = 1;
    while (pow5 <= n) {
        unsigned __int128 prod5 = pow5;

        unsigned __int128 pow3 = 1;
        /* prod53 = prod5 * pow3 must stay ≤ n;
           limit pow3 by n / prod5 to avoid overflow */
        while (pow3 <= n / prod5) {
            unsigned __int128 prod53   = prod5 * pow3;
            unsigned __int128 bound    = n / prod53;      /* maximum allowed 2‑exponent value */

            unsigned __int128 pow2 = 1;
            while (pow2 <= bound) {
                count++;
                pow2 <<= 1;
            }

            pow3 *= 3;
        }

        pow5 *= 5;
    }

    return count;
}
