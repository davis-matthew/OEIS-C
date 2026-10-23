/*
 * A033033  Numbers all of whose base 7 digits are odd.
 * https://oeis.org/A033033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the number whose base‑7 representation uses only * the
 *            digits 1,3,5. The i‑th digit (most significant first) is *
 *            obtained from the base‑3 representation of (n-1) by mapping *
 *            digit d -> 1 + 2*d
 * Range:     u8 n <= 32, u16 n <= 849, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use zero‑based index to simplify calculations */
    idx_t m = n - 1;

    /* find the number of base‑7 digits (k) for a(n) */
    idx_t pow3 = 3;          /* 3^1 */
    idx_t cumulative = 0;    /* sum_{i=1}^{k-1} 3^i */
    idx_t k = 0;

    while (1) {
        cumulative += pow3;   /* add 3^k */
        ++k;
        if (m < cumulative)
            break;           /* we found k */
        /* prevent overflow for very large n (unlikely) */
        if (pow3 > (idx_t)(-1) / 3)
            break;
        pow3 *= 3;           /* next power 3^(k+1) */
    }

    /* r is the zero‑based index within all numbers of length k */
    idx_t r = m - (cumulative - pow3);

    /* extract base‑3 digits of r (least significant first) */
    uint8_t d[64];   /* enough for any idx_t value (k ≤ 41 for 64‑bit idx_t) */
    for (idx_t i = 0; i < k; ++i) {
        d[k - 1 - i] = (uint8_t)(r % 3);
        r /= 3;
    }

    /* build the base‑7 number using digits 1,3,5 */
    seq_t value = 0;
    for (idx_t i = 0; i < k; ++i) {
        seq_t digit = (seq_t)(1 + 2 * d[i]); /* 1,3,5 */
        value = value * (seq_t)7 + digit;
    }

    return value;
}
