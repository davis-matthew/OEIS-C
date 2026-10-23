/*
 * A043766  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 3 mod 10.
 * https://oeis.org/A043766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose binary representation has a
 *            number of runs congruent to 3 modulo 10
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of runs in the binary representation of x. */
static int count_runs(idx_t x)
{
    int runs = 0;
    int prev_bit = -1;          /* ensures first bit triggers a run */
    while (x != 0) {
        int bit = x & 1;
        if (bit != prev_bit) {
            ++runs;
            prev_bit = bit;
        }
        x >>= 1;
    }
    return runs;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, safe fallback */

    idx_t count = 0;
    for (idx_t m = 1;; ++m) {
        int r = count_runs(m);
        if ((r % 10) == 3) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
