/*
 * A195552  Hypotenuses of primitive Pythagorean triples in A195550 and
 *          A195551.
 * https://oeis.org/A195552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 5, a(2) = 109, a(3) = 1189, a(4) = 6485, a(5) = 141481,
 *            a(6) = 1543321 For n ≥ 7 : a(n) = 1298 · a(n‑3) – a(n‑6)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the first six terms */
    seq_t init[6] = { (seq_t)5, (seq_t)109, (seq_t)1189,
                      (seq_t)6485, (seq_t)141481, (seq_t)1543321 };

    if (n <= 6)
        return init[n - 1];

    /* buffer will keep a(n‑6) … a(n‑1) */
    seq_t buf[6] = { init[0], init[1], init[2],
                     init[3], init[4], init[5] };

    for (idx_t i = 6; i < n; ++i) {
        /* a(n) = 1298*a(n‑3) – a(n‑6) */
        seq_t val = (seq_t)1298 * buf[3] - buf[0];

        /* shift the window: drop oldest, push new tail */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = val;
    }

    return buf[5];   /* a(n) */
}
