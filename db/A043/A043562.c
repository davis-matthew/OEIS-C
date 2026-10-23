/*
 * A043562  Number of runs in base-10 representation of n.
 * https://oeis.org/A043562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of runs in the base‑10 representation of n */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* "0" consists of a single run */
        return (seq_t)1;
    }

    /* Enough for decimal digits of any 64‑bit integer (max 20 digits) */
    unsigned digits[20];
    int len = 0;
    idx_t tmp = n;

    /* Extract digits least‑significant first */
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    /* Count runs by comparing adjacent digits from most‑significant to least‑significant */
    seq_t runs = (seq_t)1;          /* there is at least one run */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] != digits[i - 1]) {
            ++runs;                 /* a change in digit starts a new run */
        }
    }
    return runs;
}
