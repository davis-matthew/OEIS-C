/*
 * A118070  Numbers with exactly one even decimal digit.
 * https://oeis.org/A118070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force generation of numbers with exactly one even decimal digit. */
seq_t a(idx_t n) {
    seq_t num = 0;          /* current candidate number          */
    idx_t count = 0;        /* how many numbers found so far    */
    const seq_t ten = 10;   /* constant for digit extraction    */

    for (;;) {
        seq_t temp = num;
        int even = 0;

        /* Treat 0 specially – it has one even digit (0 itself). */
        if (temp == 0) {
            even = 1;
        } else {
            /* Count even digits, stop early if more than one. */
            while (temp > 0 && even <= 1) {
                int digit = (int)(temp % ten);
                if ((digit & 1) == 0) ++even;
                temp /= ten;
            }
        }

        /* If exactly one even digit, it is part of the sequence. */
        if (even == 1) {
            ++count;
            if (count == n) return num;
        }

        ++num;  /* next integer to test */
    }
}
