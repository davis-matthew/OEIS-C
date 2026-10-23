/*
 * A036559  Squared distances that cannot arise in any Barlow packing of
 *          spheres of diameter sqrt(3) (the scale being the simplest one that
 *          makes this an integer sequence).
 * https://oeis.org/A036559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m such that either m ∈
 *            {2,5,14,23,38,230} or m = 4^k * t with k ≥ 0 and t ∈ {1,7,13} or
 *            t ≡ 10 (mod 16)
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t x = 1;
    while (count < n) {
        if (x == (seq_t)2 || x == (seq_t)5 || x == (seq_t)14 ||
            x == (seq_t)23 || x == (seq_t)38 || x == (seq_t)230) {
            count++;
        } else {
            seq_t t = x;
            /* Divide by powers of 4 until not divisible */
            while (t % (seq_t)4 == (seq_t)0) {
                t = t / (seq_t)4;
            }
            /* Check the reduced base */
            if (t == (seq_t)1 || t == (seq_t)7 || t == (seq_t)13 ||
                t % (seq_t)16 == (seq_t)10) {
                count++;
            }
        }
        if (count == n) {
            return x;
        }
        x = x + (seq_t)1;
    }
    /* Should never reach here */
    return x;
}
