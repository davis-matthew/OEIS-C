/*
 * A188334  Number of nondecreasing arrangements of 4 nonzero numbers in
 *          -(n+2)..(n+2) with sum zero.
 * https://oeis.org/A188334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 64, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A188334
 * Recurrence: a(n) = 2*a(n-1) - a(n-3) - a(n-4) + 2*a(n-6) - a(n-7)
 * Initial values:  a(1)=10, a(2)=20, a(3)=37, a(4)=61, a(5)=94,
 *                  a(6)=136, a(7)=191, a(8)=257
 * The function works for any call order and for n up to the machine limit,
 * but is guaranteed to be O(n) for n≤10000. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Return zero for the offset-less case but never used. */
    if (n == 0) return (seq_t)0;

    /* Direct lookup for the first eight terms. */
    static const seq_t base_vals[8] = {
        (seq_t)10,  /* a(1) */
        (seq_t)20,  /* a(2) */
        (seq_t)37,  /* a(3) */
        (seq_t)61,  /* a(4) */
        (seq_t)94,  /* a(5) */
        (seq_t)136, /* a(6) */
        (seq_t)191, /* a(7) */
        (seq_t)257  /* a(8) */
    };

    if (n <= 8) return base_vals[n-1];

    /* Sliding window of the last eight values.  buf[0] holds a(k-7),
     * buf[1] holds a(k-6), … buf[7] holds a(k-1) when the loop
     * variable k reaches its current value.  Initially populate
     * the window with the first eight terms. */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i) buf[i] = base_vals[i];

    /* Iterate from a(9) up to a(n). */
    for (idx_t k = 9; k <= n; ++k) {
        /* Calculate indices modulo 8 for the sliding window. */
        seq_t a1 = buf[(k - 2) % 8]; /* a(k-1)   */
        seq_t a3 = buf[(k - 4) % 8]; /* a(k-3)   */
        seq_t a4 = buf[(k - 5) % 8]; /* a(k-4)   */
        seq_t a6 = buf[(k - 7) % 8]; /* a(k-6)   */
        seq_t a7 = buf[(k - 8) % 8]; /* a(k-7)   */

        /* Apply the recurrence: 2*a1 - a3 - a4 + 2*a6 - a7 */
        seq_t next = a1 + a1 - a3 - a4 + a6 + a6 - a7;

        /* Store the new value, overwriting the oldest one (a(k-7)). */
        buf[(k - 1) % 8] = next;
    }

    /* After the loop, buf[(n-1)%8] holds a(n). */
    return buf[(n - 1) % 8];
}
