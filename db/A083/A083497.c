/*
 * A083497  Multiples of 9 in which there is no common digit in successive
 *          terms.
 * https://oeis.org/A083497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th multiple of 9 such that successive terms share
 *            no common decimal digit (a(0)=9, a(1)=18, a(2)=27, …)
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned digit_mask(seq_t v) {
    unsigned mask = 0;
    while (v != 0) {
        mask |= 1u << (int)(v % 10);
        v /= 10;
    }
    return mask;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)9;

    seq_t last = (seq_t)9;
    unsigned last_mask = digit_mask(last);
    idx_t count = 0;

    seq_t cand = last + 9;
    while (count < n) {
        unsigned cand_mask = digit_mask(cand);
        if ((cand_mask & last_mask) == 0) {
            last = cand;
            last_mask = cand_mask;
            count++;
            if (count == n) return last;
        }
        cand += (seq_t)9;
    }
    return last;  /* should never reach here */
}
