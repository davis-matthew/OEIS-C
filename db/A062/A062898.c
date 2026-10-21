/*
 * A062898  Number and its reversal are both multiples of 4.
 * https://oeis.org/A062898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062898: The nth positive integer such that the number
 *        and its decimal reversal are both divisible by 4. */

/* Helper: return the decimal reversal of x. */
static seq_t reverse(uint64_t x)
{
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + (seq_t)(x % 10);
        x /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num  = 4;           /* first candidate (a(1) = 4) */

    for (;; num += 1) {
        /* test for divisibility by 4 and its reversal */
        if (num % 4 == 0) {
            seq_t rev = reverse((uint64_t)num);
            if (rev % 4 == 0) {
                count += 1;
                if (count == n)
                    return num;
            }
        }
    }
}
