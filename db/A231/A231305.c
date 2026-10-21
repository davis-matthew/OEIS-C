/*
 * A231305  Recurrence a(n) = a(n-2) + n^M for M=6, starting with a(0)=0,
 *          a(1)=1.
 * https://oeis.org/A231305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-2k)^6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 33, u64 n <= 823, u128 n <= 9999,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute x^6 using only seq_t operations */
static inline seq_t pow6(seq_t x) {
    seq_t r  = x * x;  /* x^2 */
    seq_t r4 = r * r;  /* x^4 */
    return r4 * r;     /* x^6 */
}

seq_t a(idx_t n) {
    seq_t res = 0;
    idx_t half = n / 2;
    for (idx_t i = 0; i <= half; ++i) {
        seq_t v = (seq_t)(n - 2 * i); /* v is always non‑negative */
        res += pow6(v);
    }
    return res;
}
