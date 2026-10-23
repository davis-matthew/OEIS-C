/*
 * A023713  Numbers with no 2's in their base 4 expansion.
 * https://oeis.org/A023713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained by writing (n‑1) in base 3, then interpreting
 *            its digits {0,1,2} as {0,1,3} in base 4
 * Range:     u8 n <= 81, u16 n <= 6561, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; n==0 is outside the domain, but we handle it simply. */
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t k = n - 1;          /* work with n‑1 in base‑3 */
    seq_t result = (seq_t)0;  /* accumulate the base‑4 value */
    seq_t power  = (seq_t)1;  /* 4^position */

    while (k > 0) {
        idx_t digit = k % (idx_t)3; /* base‑3 digit (0,1,2) */
        k /= (idx_t)3;

        /* map 0→0, 1→1, 2→3 */
        seq_t mapped = (digit == (idx_t)2) ? (seq_t)3 : (seq_t)digit;

        result += mapped * power;
        power  *= (seq_t)4;
    }

    return result;
}
