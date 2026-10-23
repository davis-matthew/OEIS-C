/*
 * A037883  a(n)=(1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*6^i} is the base 6
 *          representation of n and e(i) are the digits d(i) in nondecreasing
 *          order.
 * https://oeis.org/A037883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ½ * Σ |d(i) - e(i)| where Σ d(i) * 6^i is the base‑6
 *            representation of n and e(i) are the digits d(i) sorted in
 *            nondecreasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the trivial case n = 0 (though n ≥ 1 in OEIS) */
    if (n == 0) return 0;

    /* collect base‑6 digits, least significant first */
    int digits[64];
    int len = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[len++] = temp % 6;
        temp /= 6;
    }

    /* make a copy for sorting */
    int sorted[64];
    for (int i = 0; i < len; i++) sorted[i] = digits[i];

    /* simple insertion sort (array size ≤ 25 for 64‑bit values) */
    for (int i = 1; i < len; i++) {
        int v = sorted[i];
        int j = i - 1;
        while (j >= 0 && sorted[j] > v) {
            sorted[j + 1] = sorted[j];
            j--;
        }
        sorted[j + 1] = v;
    }

    /* compute the sum of absolute differences */
    seq_t sum = 0;
    for (int i = 0; i < len; i++) {
        int d = digits[i] - sorted[i];
        if (d < 0) d = -d;
        sum += (seq_t)d;
    }

    /* divide by two */
    return sum / 2;
}
