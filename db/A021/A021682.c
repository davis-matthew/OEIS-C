/*
 * A021682  Decimal expansion of 1/678.
 * https://oeis.org/A021682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A021682: digit n of the decimal expansion of 1/678 (period = 120) */
seq_t a(idx_t n)
{
    static seq_t cycle[120];
    static int initialized = 0;
    if (!initialized) {
        idx_t rem = 1;               /* current remainder */
        for (int i = 0; i < 120; ++i) {
            rem *= 10;
            seq_t d = rem / 678;    /* quotient is the next digit */
            cycle[i] = d;
            rem = rem % 678;        /* new remainder for next step */
        }
        initialized = 1;
    }
    return cycle[n % 120];
}
