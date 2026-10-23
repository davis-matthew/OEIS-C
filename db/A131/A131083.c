/*
 * A131083  Periodic sequence (26, 16, 6, 2, 6, 16, 26, 30).
 * https://oeis.org/A131083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-8); periodic sequence of period 8: 26, 16, 6, 2, 6,
 *            16, 26, 30
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1; compute (n-1) mod 8 using a power‑of‑two mask for speed */
    idx_t idx = (n - 1) & 7;

    switch (idx) {
        case 0: return (seq_t)26;  /* a(1) */
        case 1: return (seq_t)16;  /* a(2) */
        case 2: return (seq_t)6;   /* a(3) */
        case 3: return (seq_t)2;   /* a(4) */
        case 4: return (seq_t)6;   /* a(5) */
        case 5: return (seq_t)16;  /* a(6) */
        case 6: return (seq_t)26;  /* a(7) */
        case 7: return (seq_t)30;  /* a(8) */
        default: return (seq_t)0;   /* unreachable, keep compiler happy */
    }
}
