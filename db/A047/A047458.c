/*
 * A047458  Numbers that are congruent to {0, 3, 4} mod 8.
 * https://oeis.org/A047458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n >= 1, a(n) = 8 * floor((n + 2)/3) - 8 if n mod 3 == 1 8 *
 *            floor((n + 1)/3) - 5 if n mod 3 == 2 8 * floor(n/3) - 4 if n mod
 *            3 == 0 This implements the explicit description: a(3k-2) = 8k-8,
 *            a(3k-1) = 8k-5, a(3k) = 8k-4
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1 by the problem definition. */
    switch (n % 3) {
    case 0: { /* n = 3k */
        seq_t k = (seq_t)(n / 3);
        return k * 8 - 4;
    }
    case 1: { /* n = 3k-2 */
        seq_t k = (seq_t)((n + 2) / 3);
        return k * 8 - 8;
    }
    case 2: { /* n = 3k-1 */
        seq_t k = (seq_t)((n + 1) / 3);
        return k * 8 - 5;
    }
    }
    /* Should never reach here; return zero to satisfy strict return. */
    return 0;
}
