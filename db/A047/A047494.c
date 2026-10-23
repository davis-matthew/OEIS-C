/*
 * A047494  Numbers that are congruent to {0, 1, 4, 5, 7} mod 8.
 * https://oeis.org/A047494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(5k) = 8k-1, a(5k-1) = 8k-3, * a(5k-2) = 8k-4, a(5k-3) = 8k-7,
 *            * a(5k-4) = 8k-8 (k = ceil(n/5)) * * This implements OEIS
 *            sequence A047494: numbers congruent to * {0, 1, 4, 5, 7} modulo
 *            8. The index is 1‑based (a(1)=0)
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n==0 (below offset) the behaviour is unspecified;
       we return 0 for safety. */
    if (n == 0)
        return 0;

    /* k = ceil(n/5) = (n + 4) / 5 for unsigned arithmetic */
    idx_t k = (n + 4) / 5;

    /* Compute a(n) using the appropriate remainder case. */
    switch (n % 5) {
        case 1:
            return (seq_t)8 * (seq_t)k - 8;
        case 2:
            return (seq_t)8 * (seq_t)k - 7;
        case 3:
            return (seq_t)8 * (seq_t)k - 4;
        case 4:
            return (seq_t)8 * (seq_t)k - 3;
        case 0:
            return (seq_t)8 * (seq_t)k - 1;
    }
    /* Unreachable, but keep compiler happy */
    return 0;
}
