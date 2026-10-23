/*
 * A045058  Numbers whose base-4 representation contains exactly four 0's and
 *          one 2.
 * https://oeis.org/A045058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the nth integer whose base‑4 representation contains exactly
 *            four 0's and one 2 (ordered by increasing value)
 * Range:     u8 none, u16 n <= 1321, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* power of 2: 2^e */
static seq_t pow2_seq(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 2;
    return r;
}

/* binomial coefficient C(n,k) for small n,k, result fits in seq_t */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* count of numbers of length L (L>=5) with exactly four 0's and one 2, no leading zero */
static seq_t count_len(idx_t L)
{
    if (L < 5) return 0;
    /* zeros at positions 1..L-1: choose 4 out of L-1 */
    seq_t zeros_choose = binom_seq(L - 1, 4);
    /* choose the position of the 2 among remaining L-4 positions (could be first digit) */
    seq_t twos_choose = (seq_t)(L - 4);
    /* remaining positions (L-5) are free: each can be 1 or 3 */
    seq_t free_choices = pow2_seq(L - 5);
    return zeros_choose * twos_choose * free_choices;
}

/* number of completions for remaining positions
   zeros_needed: remaining zeros still to place
   twos_needed: remaining twos still to place (0 or 1)
   rem: number of free positions left (including current, if called before choosing a digit)
*/
static seq_t count_remaining(idx_t zeros_needed, idx_t twos_needed, idx_t rem)
{
    if (zeros_needed > rem || twos_needed > rem || zeros_needed + twos_needed > rem)
        return 0;
    /* choose positions for zeros */
    seq_t ways_zero = binom_seq(rem, zeros_needed);
    /* choose position for the 2 if needed */
    seq_t ways_two = 1;
    if (twos_needed == 1)
        ways_two = (seq_t)(rem - zeros_needed);
    /* remaining positions become free (1 or 3) */
    idx_t free_pos = rem - zeros_needed - twos_needed;
    seq_t ways_free = 1;
    if (free_pos > 0)
        ways_free = pow2_seq(free_pos);
    return ways_zero * ways_two * ways_free;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of defined range */

    /* determine the length L of the n-th number */
    idx_t L = 5;
    seq_t cumulative = 0;
    seq_t cntL;
    while (true)
    {
        cntL = count_len(L);
        if (n <= (idx_t)(cumulative + cntL))
            break;
        cumulative += cntL;
        ++L;
    }

    /* offset within the block of numbers of length L (0‑based) */
    seq_t offset = (seq_t)(n - cumulative - 1);

    /* current state for enumeration */
    idx_t zeros_needed = 4;
    idx_t twos_needed = 1;
    idx_t rem = L;
    seq_t value = 0;

    for (idx_t pos = 0; pos < L; ++pos)
    {
        for (int digit = 0; digit <= 3; ++digit)
        {
            /* leading zero not allowed */
            if (pos == 0 && digit == 0) continue;
            /* check availability of the digit */
            if (digit == 0 && zeros_needed == 0) continue;
            if (digit == 2 && twos_needed == 0) continue;

            /* compute parameters if this digit is chosen */
            idx_t new_zeros = zeros_needed;
            idx_t new_twos  = twos_needed;
            if (digit == 0) --new_zeros;
            else if (digit == 2) --new_twos;

            idx_t rem_after = rem - 1;
            seq_t cnt = count_remaining(new_zeros, new_twos, rem_after);

            if (offset >= cnt)
            {
                offset -= cnt;
                continue;  /* try next digit */
            }
            else
            {
                /* this digit is fixed */
                value = value * 4 + (seq_t)digit;
                zeros_needed = new_zeros;
                twos_needed  = new_twos;
                rem = rem_after;
                break;     /* move to next position */
            }
        }
    }
    return value;
}
