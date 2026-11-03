/*
 * A134808  Cyclops numbers.
 * https://oeis.org/A134808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=0; for n>1, a(n) is the (n‑1)th smallest number * that has
 *            an odd length, exactly one zero which is the middle digit, * and
 *            no other zeros. For a given k≥1 the numbers are * left *
 *            10^(k+1) + right * with left and right being k‑digit numbers
 *            over {1,…,9}. They * are ordered first by k, then...
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 993 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: construct the k‑digit number (digits 1‑9) from
 * a 0‑based index in base‑9 representation (each digit 0‑8
 * corresponds to the actual digit +1).  pow9k = 9^k. */
static seq_t
num_from_index (idx_t k, unsigned __int128 index, unsigned __int128 pow9k)
{
    seq_t   num  = 0;
    /* pow is 9^(k-1), 9^(k-2), …, 9^0 in successive iterations */
    unsigned __int128 pow = pow9k / 9; /* start with 9^(k-1) */
    for (int pos = 0; pos < (int)k; ++pos)
    {
        unsigned int digit_base9 = (unsigned int)(index / pow);
        index %= pow;
        num = num * 10 + (seq_t)(digit_base9 + 1);
        pow /= 9;       /* move to the next less significant digit */
    }
    return num;
}

/* Helper: compute 10^exp as a seq_t.  exp>=0. */
static seq_t
pow10 (int exp)
{
    seq_t x = 1;
    while (exp--) x *= 10;
    return x;
}

seq_t a (idx_t n)
{
    /* The sequence starts with 0 at a(1). */
    if (n <= 1) return 0;

    /* rem: 1‑based index among non‑zero cyclops numbers */
    unsigned __int128 rem = (unsigned __int128)(n - 1);

    /* Determine the block (value of k) containing the rem-th number. */
    unsigned __int128 pow9k = 9;   /* 9^1 for k=1 */
    idx_t          k      = 1;
    while (1)
    {
        unsigned __int128 blockCount = pow9k * pow9k;  /* 81^k */
        if (rem <= blockCount) break;
        rem -= blockCount;            /* skip entire block */
        pow9k *= 9;                    /* advance to next k */
        ++k;
    }

    /* Now rem is 1‑based inside the current block. Convert to 0‑based. */
    unsigned __int128 blockIndex = rem - 1;
    unsigned __int128 leftIdx    = blockIndex / pow9k;  /* 0‑based left index */
    unsigned __int128 rightIdx   = blockIndex % pow9k;  /* 0‑based right index */

    /* Construct left and right numbers. */
    seq_t left  = num_from_index ((idx_t)k, leftIdx, pow9k);
    seq_t right = num_from_index ((idx_t)k, rightIdx, pow9k);

    /* Assemble final cyclops number. */
    seq_t power10 = pow10 (k + 1);              /* 10^(k+1) */
    return left * power10 + right;
}
