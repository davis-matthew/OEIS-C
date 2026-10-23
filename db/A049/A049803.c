/*
 * A049803  a(n) = n mod 3 + n mod 9 + ... + n mod 3^k, where 3^k <= n <
 *          3^(k+1).
 * https://oeis.org/A049803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod 3 + n mod 9 + ... + n mod 3^k, where 3^k <= n <
 *            3^(k+1) (OEIS A049803)
 * Range:     u8 n <= 398, u16 n <= 9290, u32 n <= 9290, u64 n <= 9290,
 *            u128 n <= 9290, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for zero input */

    seq_t sum = (seq_t)0;
    seq_t sn  = (seq_t)n;           /* promote n to seq_t once */

    idx_t pow = 3;                  /* smallest power of 3 */
    while (pow <= n) {
        sum += sn % (seq_t)pow;     /* add n mod 3^i */
        if (pow > n / 3) break;     /* next multiplication would overflow */
        pow *= 3;                   /* go to next higher power of 3 */
    }

    return sum;
}
