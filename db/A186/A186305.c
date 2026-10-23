/*
 * A186305  n^((p-1)/2) (mod p) for p = 29.
 * https://oeis.org/A186305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^((p-1)/2) mod p, with p = 29 (i.e. n^14 (mod 29))
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n modulo 29 */
    idx_t rem = n % 29;
    seq_t base = (seq_t)rem;
    seq_t result = 1;
    unsigned exp = 14;          /* (p-1)/2 = 14 for p = 29 */

    while (exp) {
        if (exp & 1U)
            result = (result * base) % (seq_t)29;
        base = (base * base) % (seq_t)29;
        exp >>= 1U;
    }
    return result;
}
