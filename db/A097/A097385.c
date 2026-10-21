/*
 * A097385  a(n) = (largest digit of n)^(smallest digit of n) + n.
 * https://oeis.org/A097385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, look, nonn
 * Formula:   a(n) = (largest digit of n)^(smallest digit of n) + n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the term a(n) of OEIS sequence A097385. */
seq_t a(idx_t n)
{
    /* Special case for n = 0, where 0^0 is defined as 1. */
    if (n == 0) return 1;

    /* Find the largest and smallest decimal digits of n. */
    idx_t largest = 0, smallest = 9;
    idx_t m = n;
    while (m > 0) {
        idx_t d = m % 10;
        if (d > largest) largest = d;
        if (d < smallest) smallest = d;
        m /= 10;
    }

    /* Compute largest^smallest; exponent <= 9, so a simple loop is fine. */
    seq_t power = 1;
    for (idx_t i = 0; i < smallest; ++i) power *= (seq_t)largest;

    /* Return the sum of the power and the original number. */
    return power + (seq_t)n;
}
