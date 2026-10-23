/*
 * A032913  Numbers whose set of base-6 digits is {1,3}.
 * https://oeis.org/A032913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: the numbers whose base‑6 representation uses only the digits
 * 1 and 3.  The n‑th term is obtained by choosing the (n‑cumulative‑1)‑th
 * binary pattern of length L and mapping 0 → 1, 1 → 3 in base‑6.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but avoid UB */

    /* Find the length L of the base‑6 representation */
    idx_t len = 0;
    idx_t cumulative = 0;
    idx_t pow2 = 1;                 /* 2^0 */

    for (len = 1; ; ++len) {
        pow2 <<= 1;                 /* pow2 = 2^len */
        if (cumulative + pow2 >= n)
            break;
        cumulative += pow2;
    }

    /* Offset of the term within the group of numbers having 'len' digits */
    idx_t offset = n - cumulative - 1;   /* 0‑based */

    /* Build the decimal value from the binary pattern */
    seq_t value = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        seq_t digit = ((offset >> i) & 1) ? 3 : 1;
        value = value * 6 + digit;
    }
    return value;
}
