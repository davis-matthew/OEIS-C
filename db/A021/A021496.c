/*
 * A021496  Decimal expansion of 1/492.
 * https://oeis.org/A021496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-5) for n > 6, with a(0)=0, a(1)=0, a(2)=2, a(3)=0,
 *            a(4)=3, a(5)=2, a(6)=5
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: the sequence is periodic with period 5 starting at index 2. */

seq_t a(idx_t n) {
    static const seq_t period[5] = {2, 0, 3, 2, 5};
    if (n < 2)
        return 0;
    return period[(n - 2) % 5];
}
