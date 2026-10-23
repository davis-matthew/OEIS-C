/*
 * A037479  a(n)=Sum{d(i)*10^i: i=0,1,...,m}, where Sum{d(i)*9^i: i=0,1,...,m}
 *          is the base 9 representation of n.
 * https://oeis.org/A037479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the base-9 representation of n as a base-10
 *            number
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A037479: for index n (1-indexed), return the number obtained by
 * treating the base‑9 digits of n as decimal digits. */
seq_t a(idx_t n)
{
    /* Work on a copy to avoid modifying the caller's value */
    idx_t tmp = n;
    seq_t result = 0;
    seq_t mult   = 1;

    /* Extract base‑9 digits from least significant to most significant */
    while (tmp > 0) {
        unsigned digit = tmp % 9;   /* digit ∈ {0,…,8}  */
        result += (seq_t)digit * mult;   /* accumulate decimal value */
        mult   *= 10;                   /* next decimal place weight */
        tmp    /= 9;                     /* move to next digit */
    }

    return result;
}
