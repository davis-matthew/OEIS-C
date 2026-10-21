/*
 * A060587  A ternary code: inverse of A060583.
 * https://oeis.org/A060587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = result of applying the ternary‑code rule to n. For each
 *            base‑3 digit (from most significant to least) let p be the
 *            previous digit (p=0 for the first, most significant digit). If
 *            the current digit d equals p, the next digit of a(n) is d.
 *            Otherwise the next digit is the remaining element of...
 * Range:     u8 n <= 242, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

uint64_t to_base3_digits[64];

seq_t a(idx_t n)
{
    if (n == 0)  /* base‑3 representation is “0” */
        return 0;

    /* Extract base‑3 digits (least‑significant first). */
    int len = 0;
    idx_t t = n;
    while (t) {
        to_base3_digits[len++] = t % 3;
        t /= 3;
    }

    /* Process digits from most‑significant to least‑significant. */
    unsigned prev = 0;      /* left neighbour, 0 for the first digit */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        unsigned d = to_base3_digits[i];
        unsigned r = (d == prev) ? d : (3 - d - prev);
        result = result * 3 + r;
        prev = d;
    }
    return result;
}
