/*
 * A037876  (1/2)*Sum{|d(i)-e(i)|}, where Sum{d(i)*8^i} is base 8
 *          representation of n and e(i) are digits d(i) in nonincreasing
 *          order, for i=0,1,...,m.
 * https://oeis.org/A037876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2)*sum_{i=0}^{m} |d_i - e_i|, where d_i are the base‑8
 *            digits of n (least‑significant first) and e_i are those digits
 *            sorted in non‑increasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void sort_desc(unsigned *arr, idx_t len)
{
    for (idx_t i = 0; i < len; ++i)
        for (idx_t j = i + 1; j < len; ++j)
            if (arr[j] > arr[i]) {
                unsigned tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* indices below the offset are undefined */

    /* Extract base‑8 digits of n */
    idx_t          temp = n;
    unsigned       digits[64];
    idx_t          len  = 0;
    do {
        digits[len++] = (unsigned)(temp % (idx_t)8);
        temp /= (idx_t)8;
    } while (temp);

    /* Copy and sort the digits in non‑increasing order */
    unsigned sorted[64];
    for (idx_t i = 0; i < len; ++i) sorted[i] = digits[i];
    sort_desc(sorted, len);

    /* Sum absolute differences and divide by two */
    seq_t sum = 0;
    for (idx_t i = 0; i < len; ++i) {
        unsigned diff = (digits[i] > sorted[i]) ?
                         digits[i] - sorted[i] : sorted[i] - digits[i];
        sum += (seq_t)diff;
    }
    return sum / 2;
}
