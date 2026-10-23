/*
 * A032932  Numbers whose set of base-12 digits is {1,2}.
 * https://oeis.org/A032932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑12 digits are only 1
 *            and 2 (i.e. all base‑12 strings over {1,2} in ascending order)
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the minimal digit length k such that 2^(k+1) - 2 >= n.
       pow = 2^k, cumulative = 2^(k+1) - 2. */
    idx_t pow = 2;                    /* 2^1 */
    idx_t cumulative = pow * 2 - 2;   /* 2^(1+1) - 2 = 2 */
    idx_t k = 1;
    while (cumulative < n) {
        pow <<= 1;          /* pow = 2^(k+1) */
        ++k;
        cumulative = pow * 2 - 2;   /* 2^(k+1) - 2  */
    }

    /* Offset within the group of length k (0‑based) */
    idx_t offset = n - pow + 1;

    /* Build the base‑12 number: convert offset to binary bits,
       map 0→1, 1→2, and evaluate in base 12. */
    seq_t value = 0;
    for (idx_t i = 0; i < k; ++i) {
        value = value * 12 + (((offset >> (k - 1 - i)) & 1) ? 2 : 1);
    }
    return value;
}
