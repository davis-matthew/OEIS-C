/*
 * A005349  Niven (or Harshad, or harshad) numbers: numbers that are divisible
 *          by the sum of their digits.
 * https://oeis.org/A005349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   the n-th Niven number (natural number divisible by the sum of
 *            its digits)
 * Range:     u8 n <= 76, u16 n <= 8547, u32 n <= 9819, u64 n <= 9819,
 *            u128 n <= 9819, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t digit_sum(seq_t num) {
    seq_t sum = 0;
    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 0;
    
    while (count < (seq_t)n) {
        candidate++;
        seq_t dsum = digit_sum(candidate);
        if (candidate % dsum == 0) {
            count++;
        }
    }
    
    return candidate;
}
