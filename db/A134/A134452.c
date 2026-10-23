/*
 * A134452  Balanced ternary digital root of n.
 * https://oeis.org/A134452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = balanced‑ternary digital root of n. Compute the sum of
 *            the balanced‑ternary digits of n, and iterate this operation
 *            until the sum is in {-1,0,1}
 * Range:     i8 n <= 3764, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t balanced_digit_sum(seq_t val)
/* Returns the sum of the balanced‑ternary digits of val. */
{
    seq_t sum = 0;
    seq_t mag  = val >= 0 ? val : -val;          /* work with the magnitude   */
    while (mag != 0) {
        seq_t r = mag % 3;                       /* 0,1,2                      */
        mag   /= 3;
        if (r == 2) {                            /* digit –1 plus carry 1      */
            r   = -1;
            mag += 1;
        }
        if (val < 0) r = -r;                     /* reverse sign for negative  */
        sum += r;
    }
    return sum;
}

seq_t a(idx_t n)
/* Balanced‑ternary digital root of n (for n ≥ 0). */
{
    seq_t x = (seq_t)n;
    for (;;) {
        seq_t s = balanced_digit_sum(x);
        if (s >= -1 && s <= 1) return s;
        x = s;
    }
}
