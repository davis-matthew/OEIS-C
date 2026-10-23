/*
 * A107227  Numbers having no odd terms in their Zeckendorf representation.
 * https://oeis.org/A107227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ bit_k(n) * F_{3(k+1)} (sum of the binary bits of n
 *            times Fibonacci numbers whose indices are multiples of 3)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* precompute the first 64 Fibonacci numbers whose indices are 3,6,9,...
       they grow so fast that 64 of them cover all bits that can appear
       in a 64‑bit index n. */
    static seq_t w[64];
    static int init = 0;
    if (!init) {
        init = 1;
        seq_t f0 = 0;          /* F_0 */
        seq_t f1 = 1;          /* F_1 */
        unsigned int fib_idx = 1;   /* current Fibonacci index (f1 = F_1) */
        unsigned int w_pos = 0;     /* current weight (0‑based) */
        while (w_pos < 64) {
            while (fib_idx < 3 * (w_pos + 1)) {
                seq_t tmp = f0 + f1;
                f0 = f1;
                f1 = tmp;
                fib_idx++;
            }
            /* fib_idx == 3*(w_pos+1) now */
            w[w_pos] = f1;          /* F_{3*(w_pos+1)} */
            w_pos++;
        }
    }

    seq_t res = 0;
    idx_t tmp = n;
    unsigned int pos = 0;
    while (tmp) {
        if (tmp & 1)
            res += w[pos];
        tmp >>= 1;
        pos++;
    }
    return res;
}
