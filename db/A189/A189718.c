/*
 * A189718  Fixed point of the morphism 0->011, 1->100.
 * https://oeis.org/A189718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = parity of the number of non‑zero digits in the ternary
 *            (base‑3) representation of n. In other words, a(n) flips (0→1,
 *            1→0) for every digit that is 1 or 2 in the base‑3 expansion of
 *            n. a(0) = 0
 * Range:     u8 n <= 8703, u16 n <= 8703, u32 n <= 8703, u64 n <= 8703,
 *            u128 n <= 8703, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the nth term of OEIS A189718 (fixed point of the morphism 0→011, 1→100). */
seq_t a(idx_t n) {
    seq_t res = 0;
    while (n > 0) {
        if (n % 3)            /* digit is 1 or 2: complement the result */
            res ^= 1;
        n /= 3;
    }
    return res;
}
