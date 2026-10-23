/*
 * A045141  Numbers whose base-4 representation contains exactly one 2 and
 *          three 3's.
 * https://oeis.org/A045141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth positive integer whose base‑4 representation
   contains exactly one digit '2' and exactly three digits '3'.
   The enumeration follows the natural order of the numbers. */
#include <stdint.h>
#include <stdbool.h>

/* Return the binomial coefficient C(n,k). */
static inline seq_t comb(int n, int k)
{
    if (k < 0 || k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t r = 1;
    for (int i = 1; i <= k; ++i) {
        r *= (seq_t)(n - k + i);
        r /= (seq_t)i;
    }
    return r;
}

/* Return 2^exp as a seq_t, assuming exp is small. */
static inline seq_t pow2(unsigned int exp)
{
    seq_t r = 1;
    while (exp--)
        r <<= 1;
    return r;
}

/* Number of length‑len strings over {0,1,2,3} that contain exactly
   one '2' and three '3's, with the leading digit non‑zero. */
static inline seq_t count_len(idx_t len)
{
    if (len < 4)
        return 0;

    seq_t total = 0;

    /* Leading digit 2. */
    {
        seq_t ways = comb((int)(len - 1), 3);
        ways *= pow2((unsigned int)(len - 4));
        total += ways;
    }

    /* Leading digit 3. */
    {
        seq_t ways = (seq_t)(len - 1) * comb((int)(len - 2), 2);
        ways *= pow2((unsigned int)(len - 4));
        total += ways;
    }

    /* Leading digit 1 (only when len >= 5). */
    if (len >= 5) {
        seq_t ways = (seq_t)(len - 1) * comb((int)(len - 2), 3);
        ways *= pow2((unsigned int)(len - 5));
        total += ways;
    }

    return total;
}

/* Number of ways to arrange rem2 2's and rem3 3's among rem_pos
   positions when the remaining positions may be 0 or 1. */
static inline seq_t count_rem_cap(int rem_pos, int rem2, int rem3)
{
    if (rem2 < 0 || rem3 < 0 || rem2 + rem3 > rem_pos)
        return 0;

    seq_t ways = comb(rem_pos, rem2);
    ways *= comb(rem_pos - rem2, rem3);
    int exp = rem_pos - rem2 - rem3;
    ways *= pow2((unsigned int)exp);
    return ways;
}

seq_t a(idx_t n)
{
    /* Find the digit length of the nth term. */
    idx_t target = n;
    idx_t len = 4;
    while (1) {
        seq_t cnt = count_len(len);
        if (target > cnt) {
            target -= (idx_t)cnt;
            ++len;
        } else
            break;
    }

    /* Unranking within numbers of length `len`. */
    seq_t offset = (seq_t)target;          /* 1‑based index within this length */
    int rem2 = 1, rem3 = 3;                /* remaining counts of 2’s and 3’s */
    seq_t value = 0;

    for (int pos = (int)len - 1; pos >= 0; --pos) {
        for (int d = 0; d <= 3; ++d) {
            if (pos == (int)len - 1 && d == 0)
                continue;                   /* no leading zero */

            int n2 = rem2;
            int n3 = rem3;
            if (d == 2)
                --n2;
            if (d == 3)
                --n3;
            if (n2 < 0 || n3 < 0)
                continue;

            seq_t ways = count_rem_cap(pos, n2, n3);
            if (ways >= offset) {
                value = (value << 2) + (seq_t)d;
                rem2 = n2;
                rem3 = n3;
                break;
            } else {
                offset -= (seq_t)ways;
            }
        }
    }
    return value;
}
