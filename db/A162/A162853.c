/*
 * A162853  Take the binary representation of n. Reduce by one digit every run
 *          (completely of either 0's or 1's) of an even number of digits.
 *          Increase by one digit every run of an odd number of digits in the
 *          binary representation of n (where this added digit has the same
 *          value that makes up the rest of the r...
 * https://oeis.org/A162853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 340, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A162853.
   For each run of equal bits in the binary representation of n,
   change its length to r-1 if r is even, or to r+1 if r is odd
   (thus removing one bit from even‑length runs, adding one bit to odd‑length runs),
   keeping the bit value, and then return the decimal value of the
   resulting bit string.  The sequence is a self‑inverse permutation
   of the nonnegative integers. */

seq_t a(idx_t n)
{
    /* Special case n = 0 -> binary "0" -> run length 1 (odd) -> "00" -> 0 */
    if (n == 0)
        return 0;

    /* Find number of bits required to represent n (most significant bit index + 1) */
    idx_t temp = n;
    int num_bits = 0;
    while (temp) {
        ++num_bits;
        temp >>= 1;
    }

    seq_t result = 0;         /* Accumulate output bits here */
    int curr_bit = 1;         /* The most significant bit is always 1 for n > 0 */
    int run_len  = 1;         /* We have seen the first '1' */
    /* Process remaining bits from MSB-1 down to LSB */
    for (int i = num_bits - 2; i >= 0; --i) {
        int bit = (n >> i) & 1;
        if (bit == curr_bit) {
            ++run_len;
        } else {
            /* End of current run: emit adjusted run */
            int add_len = run_len + ((run_len & 1) ? 1 : -1);
            for (int j = 0; j < add_len; ++j)
                result = (result << 1) | (seq_t)curr_bit;
            curr_bit = bit;
            run_len  = 1;
        }
    }
    /* Emit the last run */
    int add_len = run_len + ((run_len & 1) ? 1 : -1);
    for (int j = 0; j < add_len; ++j)
        result = (result << 1) | (seq_t)curr_bit;

    return result;
}
