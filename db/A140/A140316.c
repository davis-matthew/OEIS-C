/*
 * A140316  Write n in base 3, delete 0's.
 * https://oeis.org/A140316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal number obtained by writing n in base 3 and
 *            deleting all zero digits
 * Range:     u8 n <= 39, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* the sequence is defined for n ≥ 1 */

    // Buffer for base‑3 digits (most digits will fit in 64 even for
    // the largest 64‑bit n).
    unsigned int digs[128];
    int top = 0;
    idx_t tmp = n;

    // Extract base‑3 digits least‑significant first.
    while (tmp > 0) {
        unsigned int d = tmp % 3U;
        digs[top++] = d;
        tmp /= 3U;
    }

    // Build the decimal result from most‑significant kept digit.
    seq_t result = 0;
    for (int i = top - 1; i >= 0; --i) {
        unsigned int d = digs[i];
        if (d != 0) {
            result = result * 10 + d;
        }
    }
    return result;
}
