/*
 * A171594  Squares in lunar arithmetic in base 7 written in base 10.
 * https://oeis.org/A171594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the lunar square of n in base 7 and returns the value in decimal. */

seq_t a(idx_t n)
{
    /* Convert n to base‑7 digits (least significant first). */
    int digits[64];
    idx_t len = 0;
    idx_t tmp = n;
    do {
        digits[len++] = (int)(tmp % 7);
        tmp /= 7;
    } while (tmp > 0);

    /* Lunar convolution: result has 2*len‑1 digits. */
    idx_t res_len = 2 * len - 1;
    int res[128];
    for (idx_t k = 0; k < res_len; ++k) res[k] = 0;

    for (idx_t i = 0; i < len; ++i) {
        int di = digits[i];
        for (idx_t j = 0; j < len; ++j) {
            int dj = digits[j];
            int val = di < dj ? di : dj;        /* = min(di, dj) */
            idx_t pos = i + j;
            if (val > res[pos]) res[pos] = val; /* = max over all pairs giving this position */
        }
    }

    /* Convert the resulting base‑7 number back to decimal. */
    seq_t val = 0;
    for (idx_t pos = res_len; pos > 0; --pos) {
        val = val * 7 + (seq_t)res[pos - 1];
    }
    return val;
}
