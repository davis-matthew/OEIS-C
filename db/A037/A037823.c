/*
 * A037823  Number of i such that d(i) > d(i-1), where Sum_{i=0..m} d(i)*8^i
 *          is the base-8 representation of n.
 * https://oeis.org/A037823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of adjacent digit pairs (d_i, d_{i-1}) in the
 *            base‑8 representation of n * for which d_i > d_{i-1}. Here d_0
 *            is the most significant digit and d_{k-1} the least
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* If n == 0 we return 0, although the sequence is defined only for n>=1. */
    if (n == 0)
        return 0;

    /* Store the base‑8 digits least significant first.  A 64‑bit value needs at most 22 digits in base 8. */
    unsigned char digits[32];
    int pos = 0;
    idx_t tmp = n;

    /* Extract digits using bit operations: n % 8 == n & 7,  n / 8 == n >> 3. */
    while (tmp != 0) {
        digits[pos++] = (unsigned char)(tmp & 7u);
        tmp >>= 3;
    }

    seq_t count = 0;
    /* Scan from the most significant digit (index pos-1) downwards, comparing each with the next lower digit. */
    for (int i = pos - 1; i > 0; --i) {
        if (digits[i] > digits[i - 1])
            ++count;
    }

    return count;
}
