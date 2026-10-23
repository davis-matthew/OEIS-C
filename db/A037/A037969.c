/*
 * A037969  Numbers whose maximal base-2 run length is 2.
 * https://oeis.org/A037969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Brute‑force search for numbers whose maximal base‑2 run length is exactly 2. */
seq_t a(idx_t n)
{
    idx_t pos = 0;
    for (seq_t cand = 1;; ++cand) {
        seq_t y = cand;
        int max_run = 0;
        int cur_run = 0;
        int last_bit = -1;
        while (y > 0) {
            int bit = (int)(y & (seq_t)1);
            if (bit == last_bit) {
                cur_run++;
            } else {
                if (last_bit != -1 && cur_run > max_run) max_run = cur_run;
                cur_run = 1;
                last_bit = bit;
            }
            y >>= 1;
        }
        if (cur_run > max_run) max_run = cur_run;
        if (max_run == 2) {
            ++pos;
            if (pos == n) return cand;
        }
    }
}
