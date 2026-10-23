/*
 * A047352  Numbers that are congruent to {0, 2} mod 7.
 * https://oeis.org/A047352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7/3)*floor(3*n/2)) (n ≥ 1)
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent direct expression (O(1) time, no memoisation). */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  n=0 is undefined,
       but we return 0 for safety. */
    if (n == 0) {
        return 0;
    }

    seq_t N = (seq_t)n;               /* promote index to seq_t */

    if (n & 1ULL) {                    /* n is odd: a = 7*((n-1)/2) */
        return 7 * (N - 1) / 2;
    } else {                           /* n is even: a = 7*n/2 - 5 */
        return 7 * N / 2 - 5;
    }
}
