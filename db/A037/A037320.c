/*
 * A037320  Numbers whose base-4 and base-9 expansions have the same digit
 *          sum.
 * https://oeis.org/A037320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037320: Numbers whose base-4 and base-9 expansions have the same digit sum.
   This implementation searches integers sequentially and returns the nth term. */

/* Helper: digit sum of x in given base. */
static inline seq_t digit_sum(seq_t x, unsigned base) {
    seq_t sum = 0;
    while (x > 0) {
        sum += x % base;
        x   /= base;
    }
    return sum;
}

seq_t a(idx_t n) {
    /* n is 1-indexed: a(1) is the first term */
    seq_t goal   = (seq_t)n;
    seq_t count  = 0;
    seq_t number = 1;

    while (count < goal) {
        seq_t s4 = digit_sum(number, 4);
        seq_t s9 = digit_sum(number, 9);
        if (s4 == s9) {
            count++;
            if (count == goal)
                return number;
        }
        number++;
    }
    return 0; /* should never reach here for n >= 1 */
}
