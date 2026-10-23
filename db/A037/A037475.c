/*
 * A037475  a(n)=Sum{d(i)*9^i: i=0,1,...,m}, where Sum{d(i)*7^i: i=0,1,...,m}
 *          is the base 7 representation of n.
 * https://oeis.org/A037475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum{ d(i)*9^i : i = 0,1,…,m } where Sum{ d(i)*7^i : i =
 *            0,1,…,m } is the base‑7 representation of n
 * Range:     u8 n <= 157, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A037475: interpret the base‑7 digits of n as a base‑9 number */
seq_t a(idx_t n)
{
    seq_t result = 0u;   /* the accumulated base‑9 value */
    seq_t base   = 1u;   /* current power of 9 : 9^i */

    idx_t temp = n;      /* work on a copy of n */

    while (temp > 0) {
        idx_t digit = temp % 7;          /* next base‑7 digit (0..6) */
        if (digit)                       /* add digit * 9^i to the result */
            result += (seq_t)digit * base;
        temp /= 7;                       /* move to the next digit */
        base *= 9;                       /* next power of 9 */
    }

    return result;
}
