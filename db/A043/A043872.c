/*
 * A043872  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 5 mod 10.
 * https://oeis.org/A043872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer such that * when written in
 *            base 4, the number of runs of consecutive equal * digits is
 *            congruent to 5 (mod 10)
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int runs_in_base4(seq_t v)
{
    if (v == 0) return 1;            /* base‑4 "0" has one run */
    int runs = 0;
    int prev = -1;
    while (v > 0) {
        int d = (int)(v % 4);        /* current least significant base‑4 digit */
        if (d != prev) {
            runs++;
            prev = d;
        }
        v /= 4;
    }
    return runs;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; for any n ≥ 1 return the nth term of A043872. */
    seq_t cand = 1;          /* current candidate number */
    idx_t count = 0;         /* how many qualifying numbers found so far */

    while (count < n) {
        if (runs_in_base4(cand) % 10 == 5) {
            count++;
            if (count == n)
                return cand;
        }
        cand++;
    }

    /* Should never reach here – but return last candidate for completeness. */
    return cand;
}
