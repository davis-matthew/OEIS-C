/*
 * A070772  b+c+d+e+f where b>=c>=d>=e>=f>=0 ordered by b then c then d then e
 *          then f.
 * https://oeis.org/A070772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the sum b+c+d+e+f of the lexicographically
 * nth (0‑based) non‑increasing 5‑tuple (b≥c≥d≥e≥f≥0).
 * The tuple sequence is enumerated with b as the outermost index,
 * then c, d, e and f.
 * Example: a(0)=0 from (0,0,0,0,0), a(1)=1 from (1,0,0,0,0), a(2)=2 from (1,1,0,0,0), …
 */
seq_t a(idx_t n) {
    idx_t idx = 0;                 /* current position in enumeration */
    for (seq_t b = 0; ; ++b) {     /* outermost component */
        for (seq_t c = 0; c <= b; ++c) {
            for (seq_t d = 0; d <= c; ++d) {
                for (seq_t e = 0; e <= d; ++e) {
                    for (seq_t f = 0; f <= e; ++f) {
                        if (idx == n) {
                            return b + c + d + e + f;  /* sum of the tuple */
                        }
                        ++idx;
                    }
                }
            }
        }
    }
}
