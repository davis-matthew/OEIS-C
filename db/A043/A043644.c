/*
 * A043644  Numbers whose base-10 representation has exactly 8 runs.
 * https://oeis.org/A043644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) lists the n-th smallest integer whose decimal representation has exactly 8 runs.
   The function simply scans integers from 10^7 upward, counting the number
   of run changes in each decimal expansion until the n-th such number is found. */

static inline int count_runs(seq_t x)
{
    /* Counts the number of contiguous blocks of identical digits in x. */
    if (x == 0) return 1;          /* special case, not used here */
    int prev_digit = -1;
    int runs = 0;
    while (x != 0) {
        int digit = (int)(x % 10);
        if (digit != prev_digit) {
            runs++;
            prev_digit = digit;
        }
        x /= 10;
    }
    return runs;
}

seq_t a(idx_t n)
{
    /* Find the n-th integer (n >= 1) that has exactly 8 runs in base‑10. */
    if (n == 0) return 0;          /* undefined but safe guard */
    seq_t val = (seq_t)10000000;    /* smallest 8‑digit number (10^7) */
    idx_t count = 0;

    while (1) {
        if (count_runs(val) == 8) {
            count++;
            if (count == n)
                return val;
        }
        val++;
    }
}
