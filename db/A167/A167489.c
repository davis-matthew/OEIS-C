/*
 * A167489  Product of run lengths in binary representation of n.
 * https://oeis.org/A167489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the lengths of runs of identical bits in the
 *            binary representation of n (a(0) = 1)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the binary representation is "0", a single run of length 1. */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* Determine the bit of the first run (least significant bit). */
    int current_bit = (int)(m & (idx_t)1);
    idx_t run_len = 1;
    m >>= 1;                       /* Skip the first bit already processed. */

    /* Scan the remaining bits from least significant to most significant. */
    while (m != (idx_t)0) {
        int bit = (int)(m & (idx_t)1);
        if (bit == current_bit) {
            run_len++;              /* Continue the current run. */
        } else {
            /* End of current run: multiply its length into the result. */
            result *= (seq_t)run_len;
            current_bit = bit;
            run_len    = 1;
        }
        m >>= 1;                   /* Move to the next bit. */
    }

    /* Multiply the length of the final run. */
    result *= (seq_t)run_len;
    return result;
}
