/*
 * A105791  Trajectory of 1 under the morphism 1->{1, 2, 4, 2, 1}, 2->{4, 3,
 *          1, 3, 4}, 3->{2, 1, 3, 1, 2}, 4->{3, 4, 2, 4, 3}.
 * https://oeis.org/A105791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth symbol (1‑4) in the infinite word obtained by *
 *            repeatedly applying the 5‑letter morphism * 1 → 1 2 4 2 1 * 2 →
 *            4 3 1 3 4 * 3 → 2 1 3 1 2 * 4 → 3 4 2 4 3 * The sequence can be
 *            found by writing n in base 5 and following * the digits through
 *            the morphism starting from symbol 1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Transition table: rows correspond to symbols 1‑4, columns to digits 0‑4 */
    static const unsigned char trans[4][5] = {
        {1, 2, 4, 2, 1},   /* 1 → */
        {4, 3, 1, 3, 4},   /* 2 → */
        {2, 1, 3, 1, 2},   /* 3 → */
        {3, 4, 2, 4, 3}    /* 4 → */
    };

    unsigned cur = 1;                   /* start symbol */
    unsigned digits[64];                /* base‑5 digits of n */
    idx_t t = n;
    int len = 0;

    /* Extract base‑5 digits (least significant first) */
    do {
        digits[len++] = (unsigned)(t % 5);
        t /= 5;
    } while (t != 0);

    /* Process digits from most significant to least */
    for (int i = len - 1; i >= 0; --i) {
        unsigned d = digits[i];
        unsigned idx = cur - 1;          /* 0‑based index into trans */
        cur = trans[idx][d];
    }

    return (seq_t)cur;
}
