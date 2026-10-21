/*
 * A233612  Number of (n+2)X(2+2) 0..2 arrays with no increasing sequence of
 *          length 3 horizontally or diagonally downwards.
 * https://oeis.org/A233612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 75*a(n-1) - 279*a(n-2) - 4023*a(n-3) + 15264*a(n-4) +
 *            56196*a(n-5) - 228096*a(n-6) + 186624*a(n-7) for n ≥ 8 Initial
 *            values: a(1) = 395955 a(2) = 27826929 a(3) = 1955165211 a(4) =
 *            137370562281 a(5) = 9651696380883 a(6) = 678130944334209 a(7) =
 *            47645673900633963
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases for n ≤ 7 */
    if (n == 1) return (seq_t)395955ULL;
    if (n == 2) return (seq_t)27826929ULL;
    if (n == 3) return (seq_t)1955165211ULL;
    if (n == 4) return (seq_t)137370562281ULL;
    if (n == 5) return (seq_t)9651696380883ULL;
    if (n == 6) return (seq_t)678130944334209ULL;
    if (n == 7) return (seq_t)47645673900633963ULL;

    /* Use recurrence for n ≥ 8 */
    /* Keep the last seven terms a(n-1) … a(n-7) in variables s1 … s7  */
    seq_t s1 = (seq_t)47645673900633963ULL;   /* a(7)   */
    seq_t s2 = (seq_t)678130944334209ULL;     /* a(6)   */
    seq_t s3 = (seq_t)9651696380883ULL;       /* a(5)   */
    seq_t s4 = (seq_t)137370562281ULL;        /* a(4)   */
    seq_t s5 = (seq_t)1955165211ULL;          /* a(3)   */
    seq_t s6 = (seq_t)27826929ULL;            /* a(2)   */
    seq_t s7 = (seq_t)395955ULL;              /* a(1)   */

    for (idx_t i = 8; i <= n; ++i) {
        /* Compute a(i) = 75*s1 - 279*s2 - 4023*s3 + 15264*s4 + 56196*s5
                     - 228096*s6 + 186624*s7                       */
        seq_t pos = (seq_t)75  * s1
                  + (seq_t)15264 * s4
                  + (seq_t)56196 * s5
                  + (seq_t)186624 * s7;

        seq_t neg = (seq_t)279   * s2
                  + (seq_t)4023  * s3
                  + (seq_t)228096 * s6;

        seq_t next = pos - neg;

        /* Shift for next iteration */
        s7 = s6;
        s6 = s5;
        s5 = s4;
        s4 = s3;
        s3 = s2;
        s2 = s1;
        s1 = next;
    }

    return s1; /* a(n) */
}
