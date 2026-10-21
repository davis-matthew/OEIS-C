/*
 * A054796  Numbers of form 17+n^2+n or 17+2*n^2.
 * https://oeis.org/A054796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 432, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers of the form 17 + n^2 + n or 17 + 2*n^2,
   sorted ascending, without duplicates. */
seq_t a(idx_t n)
{
    idx_t i = 0, j = 0;
    seq_t val1 = 17, val2 = 17;   /* values for i=0 and j=0 */
    idx_t count = 0;

    for (;;) {
        if (val1 < val2) {
            ++count;
            if (count == n) return val1;
            ++i;
            val1 = 17 + (seq_t)i * ((seq_t)i + (seq_t)1);
        } else if (val2 < val1) {
            ++count;
            if (count == n) return val2;
            ++j;
            val2 = 17 + (seq_t)2 * ((seq_t)j * (seq_t)j);
        } else { /* equal values – output once and advance both */
            ++count;
            if (count == n) return val1;  /* val1 == val2 */
            ++i;
            ++j;
            val1 = 17 + (seq_t)i * ((seq_t)i + (seq_t)1);
            val2 = 17 + (seq_t)2 * ((seq_t)j * (seq_t)j);
        }
    }
}
