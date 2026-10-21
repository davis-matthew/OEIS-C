/*
 * A073221  The terms of A073213 (sums of two powers of 17) divided by 2.
 * https://oeis.org/A073221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (17^m + 17^n)/2 with 0 ≤ m ≤ n, enumerated by increasing
 *            n then m
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* size_t, NULL */

#define MAX_EXP 20000

/* Pre‑compute powers of 17 up to MAX_EXP.  The array is filled lazily. */
static seq_t pow17[MAX_EXP + 1];
static idx_t pow17_len = 0;

static void ensure_powers(idx_t exp)
{
    if (pow17_len == 0) {
        pow17[0] = (seq_t)1;
        pow17_len = 1;
    }
    while (pow17_len <= exp) {
        pow17[pow17_len] = pow17[pow17_len - 1] * (seq_t)17;
        pow17_len++;
    }
}

/* Return the n‑th term of the sequence (0‑based). */
seq_t a(idx_t n)
{
    /* Find the block number b such that
       sum_{k=0}^{b-1} (k+1) ≤ n < sum_{k=0}^{b} (k+1). */
    idx_t b = 0;
    while (((unsigned __int128)(b + 1) * (b + 2)) / 2 <= n && b < MAX_EXP) {
        b++;
    }

    /* Index of m within the block: 0 ≤ m ≤ b. */
    idx_t cum_before = ((unsigned __int128)b * (b + 1)) / 2;
    idx_t m = n - cum_before;

    /* Prepare the required powers of 17. */
    if (b > MAX_EXP || m > MAX_EXP) return 0;   /* out of pre‑compute range */
    ensure_powers(b);
    ensure_powers(m);

    /* Compute (17^m + 17^b)/2. */
    return (pow17[m] + pow17[b]) / (seq_t)2;
}
