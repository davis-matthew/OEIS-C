/*
 * A026147  a(n) = position of n-th 1 in A001285 or A010059 (Thue-Morse
 *          sequence).
 * https://oeis.org/A026147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + nth evil number (evil numbers: non‑negative integers
 *            with an even number of 1s in binary)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline unsigned popcount64(idx_t x) {
    unsigned c = 0;
    while (x) {
        c += (unsigned)(x & (idx_t)1);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t i = 0; ; ++i) {
        if ((popcount64(i) & 1) == 0) {            /* even popcount  -> evil number */
            ++count;
            if (count == n)
                return (seq_t)(i + 1);           /* positions are 1‑based */
        }
    }
}
