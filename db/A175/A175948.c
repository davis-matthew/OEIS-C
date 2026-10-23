/*
 * A175948  Let @ denote binary concatenation. Then a(n) =
 *          A175945(n)@A175946(n).
 * https://oeis.org/A175948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = concatenate the run lengths of 1's and 0's in the binary
 *            representation of n, interpret that list as alternating run
 *            lengths starting with a run of 1's
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* guard – the sequence is defined for n >= 1 */

    /* Find index (0‑based) of the most significant 1‑bit. */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++msb;
    }

    /* Arrays to store run lengths of 1's and 0's; 64 bits → at most 64 runs. */
    idx_t ones[65];
    idx_t zeros[65];
    idx_t ones_c = 0;
    idx_t zeros_c = 0;

    /* Scan bits from MSB to LSB and collect runs. */
    idx_t cur_bit = (n >> msb) & 1;
    idx_t run_len = 1;

    for (idx_t i = msb; i > 0; --i) {
        idx_t bit = (n >> (i - 1)) & 1;
        if (bit == cur_bit) {
            ++run_len;
        } else {
            if (cur_bit == 1)
                ones[ones_c++] = run_len;
            else
                zeros[zeros_c++] = run_len;
            cur_bit = bit;
            run_len = 1;
        }
    }
    /* Final run after the loop */
    if (cur_bit == 1)
        ones[ones_c++] = run_len;
    else
        zeros[zeros_c++] = run_len;

    /* Construct the new binary number from the concatenated run lengths,
       alternating symbols starting with 1. */
    seq_t result = 0;
    idx_t cur_val = 1;  /* current run bit (1 then 0 then 1 ...) */

    for (idx_t k = 0; k < ones_c; ++k) {
        idx_t l = ones[k];
        while (l--) {
            result = (result << 1) | (seq_t)cur_val;
        }
        cur_val ^= 1;
    }
    for (idx_t k = 0; k < zeros_c; ++k) {
        idx_t l = zeros[k];
        while (l--) {
            result = (result << 1) | (seq_t)cur_val;
        }
        cur_val ^= 1;
    }

    return result;
}
