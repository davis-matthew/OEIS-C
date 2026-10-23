/*
 * A037694  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,2,0,3.
 * https://oeis.org/A037694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the integer whose decimal expansion consists of the
 *            first n terms of the periodic sequence 1,2,0,3. The
 *            implementation builds the number by processing groups of four
 *            digits for speed
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the repeating pattern */
    static const unsigned char pat[4] = {1, 2, 0, 3};
    seq_t val = 0;

    /* process complete 4‑digit blocks */
    idx_t full = n / 4;
    for (idx_t i = 0; i < full; ++i) {
        val = val * (seq_t)10000 + (seq_t)1203;
    }

    /* process remaining 1–3 digits */
    idx_t rem = n % 4;
    for (idx_t k = 0; k < rem; ++k) {
        val = val * (seq_t)10 + (seq_t)pat[k];
    }

    return val;
}
