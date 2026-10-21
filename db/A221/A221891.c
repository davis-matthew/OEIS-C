/*
 * A221891  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor, without
 *          2-loops or left turns.
 * https://oeis.org/A221891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2) + 5*a(n-3) - 3*a(n-4) Initial values:
 *            a(1)=0, a(2)=0, a(3)=8, a(4)=37
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MEMO_CAP 10001   /* maximum n we store in static memo */

static seq_t memo[MEMO_CAP];
static idx_t memo_max = 0;
static bool memo_inited = false;

static void init_memo(void)
{
    /* indices 0..4: 0 unused, a(1)..a(4) */
    memo[0] = (seq_t)0;
    memo[1] = (seq_t)0;
    memo[2] = (seq_t)0;
    memo[3] = (seq_t)8;
    memo[4] = (seq_t)37;
    memo_max = 4;
    memo_inited = true;
}

/* Compute a(n) for any n >= 1 */
seq_t a(idx_t n)
{
    /* Handle first four terms directly */
    if (n == 1 || n == 2)
        return (seq_t)0;
    if (n == 3)
        return (seq_t)8;
    if (n == 4)
        return (seq_t)37;

    /* If n fits in the static memo, use it */
    if (n < MEMO_CAP) {
        if (!memo_inited)
            init_memo();

        if (n <= memo_max)
            return memo[n];

        /* Extend memo up to n */
        for (idx_t i = memo_max + 1; i <= n; ++i) {
            seq_t a1 = memo[i - 1];
            seq_t a2 = memo[i - 2];
            seq_t a3 = memo[i - 3];
            seq_t a4 = memo[i - 4];
            memo[i] = a1 * (seq_t)5 - a2 * (seq_t)2 + a3 * (seq_t)5 - a4 * (seq_t)3;
        }
        memo_max = n;
        return memo[n];
    }

    /* For n >= MEMO_CAP, compute iteratively using only the last 4 values */
    seq_t last4[4];
    last4[0] = (seq_t)0;   /* a(i-4) when i = 5 */
    last4[1] = (seq_t)0;   /* a(i-3) */
    last4[2] = (seq_t)8;   /* a(i-2) */
    last4[3] = (seq_t)37;  /* a(i-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = last4[3] * (seq_t)5
                     - last4[2] * (seq_t)2
                     + last4[1] * (seq_t)5
                     - last4[0] * (seq_t)3;
        /* Shift the window */
        last4[0] = last4[1];
        last4[1] = last4[2];
        last4[2] = last4[3];
        last4[3] = next;
    }
    return last4[3];
}
