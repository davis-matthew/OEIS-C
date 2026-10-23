/*
 * A031483  Numbers whose base-7 representation has one more 0 than 6's.
 * https://oeis.org/A031483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: brute‑force search for numbers whose base‑7 representation
   contains one more 0 than 6.  The function returns the n‑th such number
   (a(1) = 7). */

seq_t a(idx_t n)
{
    /* start with the first possible candidate, 7₁₀ = 10₇ */
    seq_t val = 7;
    idx_t count = 0;

    while (1) {
        seq_t temp = val;
        int zeros = 0;
        int sixes = 0;

        /* count digits of val in base 7 */
        do {
            int digit = (int)(temp % 7);
            if (digit == 0)       zeros++;
            else if (digit == 6)  sixes++;
            temp /= 7;
        } while (temp != 0);

        if (zeros == sixes + 1) {
            count++;
            if (count == n) return val;
        }

        /* move to next integer */
        val++;
    }
}
