/*
 * A037890  a(n)=(1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*4^i} is the base 4
 *          representation of n and e(i) are the digits d(i) in reverse order.
 * https://oeis.org/A037890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0}^{⌊(k-1)/2⌋} |d_i - d_{k-1-i}|
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d_i are the base‑4 digits of n (least significant first). */
seq_t a(idx_t n)
{
    /* Work with up to 64 base‑4 digits (more than enough for 64‑bit n). */
    unsigned char digits[64];
    idx_t len = 0;
    idx_t t = n;

    /* Extract digits in base 4. */
    while (t != 0) {
        digits[len++] = (unsigned char)(t % 4);
        t /= 4;
    }

    /* Compute the sum of absolute differences of symmetric digits. */
    seq_t sum = 0;
    for (idx_t i = 0; i < len / 2; ++i) {
        int diff = (int)digits[i] - (int)digits[len - 1 - i];
        if (diff < 0)
            diff = -diff;
        sum += diff;
    }

    return sum;
}
