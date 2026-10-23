/*
 * A037991  Maximal base 7 run length is 4.
 * https://oeis.org/A037991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑7 representation *
 *            has a maximal run of identical digits equal to 4. This is
 *            computed by * brute‑force enumeration and checking each
 *            candidate for run length
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t num = 1;            /* start from 1 (a(1) is the first term listed) */
    idx_t count = 0;          /* how many terms found so far */

    while (1) {
        /* Examine sequence number 'num' */
        seq_t temp = num;
        int max_run = 0;
        int current_run = 0;
        int prev_digit = -1;  /* sentinel value for no previous digit */

        while (temp != 0) {
            int digit = (int)(temp % 7);   /* obtain least significant base‑7 digit */
            if (digit == prev_digit) {
                current_run++;
            } else {
                if (current_run > max_run) max_run = current_run;
                prev_digit = digit;
                current_run = 1;
            }
            temp = temp / 7;
        }
        if (current_run > max_run) max_run = current_run;  /* last run */

        if (max_run == 4) {              /* record term if run length is exactly 4 */
            count++;
            if (count == n) return num;
        }

        /* advance to next integer, handling up to arbitrary precision */
        num += 1;
    }
}
