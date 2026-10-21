/*
 * A073359  Nested floor product of n and fractions (2k+2)/(2k+1) for all
 *          k>=0, divided by 2.
 * https://oeis.org/A073359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = (A000960(n+1)-1)/2
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate the nested floor product until it stabilises,
   then return half the stabilised value. */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    idx_t k   = 0;
    while (1) {
        seq_t num = val * ((seq_t)(2 * k + 2));
        seq_t den = (seq_t)(2 * k + 1);
        seq_t next = num / den;
        if (next == val)
            break;
        val = next;
        ++k;
    }
    return val / 2;
}
