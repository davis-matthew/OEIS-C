/*
 * A057651  a(n) = (3*5^n - 1)/2.
 * https://oeis.org/A057651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*5^n - 1)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 5^n */
    seq_t power = 1;
    for (idx_t i = 0; i < n; ++i) {
        power *= 5;
    }

    /* For small seq_t (≤64 bit) use a 128‑bit intermediate to avoid overflow */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 temp = (unsigned __int128)power * 3;
        temp -= 1;
        temp /= 2;
        return (seq_t)temp;            /* cast back to seq_t */
    }

    /* For larger or arbitrary‑precision seq_t types the arithmetic is exact */
    return (power * 3 - 1) / 2;
}
