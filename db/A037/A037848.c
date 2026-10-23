/*
 * A037848  a(n)=Sum{d(i-1)-d(i): d(i)<d(i-1), i=1,...,m}, where Sum{d(i)*7^i:
 *          i=0,1,...,m} is the base 7 representation of n.
 * https://oeis.org/A037848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=1}^{m} (d(i-1)-d(i)) when d(i) < d(i-1), where d(i)
 *            are the base‑7 digits of n (n = Σ d(i)·7^i)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    unsigned char digits[20];      /* sufficient for 64‑bit numbers in base 7 */
    idx_t temp = n;
    int m = 0;

    /* Extract base‑7 digits (least significant first) */
    while (temp > 0) {
        digits[m++] = temp % 7;
        temp /= 7;
    }

    /* Sum the drops from one digit to the next higher digit */
    for (int i = 1; i < m; ++i) {
        if (digits[i] < digits[i - 1]) {
            sum += (seq_t)(digits[i - 1] - digits[i]);
        }
    }

    return sum;
}
