/*
 * A153837  Year of ratification of the n-th Amendment to the United States
 *          Constitution.
 * https://oeis.org/A153837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = year of ratification of the n‑th Amendment to * the
 *            United States Constitution
 * Range:     u8 none, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t years[] = {
        0,
        /*  1–10 */ 1791, 1791, 1791, 1791, 1791, 1791, 1791, 1791, 1791, 1791,
        /* 11–20 */ 1795, 1804, 1865, 1868, 1870, 1913, 1913, 1933, 1920, 1933,
        /* 21–27 */ 1933, 1951, 1961, 1964, 1967, 1971, 1992
    };
    idx_t size = sizeof(years) / sizeof(years[0]); /* includes dummy 0 index */
    return n < size ? years[n] : 0;
}
