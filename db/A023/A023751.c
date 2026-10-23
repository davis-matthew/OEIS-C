/*
 * A023751  Plaindromes: numbers whose digits in base 9 are in nondecreasing
 *          order.
 * https://oeis.org/A023751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth number whose base‑9 digits are nondecreasing
 *            (PLAINDROMES)
 * Range:     u8 n <= 108, u16 n <= 1287, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* combinatorial nCk for small n,k (fits in idx_t), returns seq_t */
static seq_t choose(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Zero is a special first term */
    if (n == (idx_t)1) return (seq_t)0;

    /* 0‑based index of positive numbers (exclude the initial 0) */
    idx_t rem_idx = n - (idx_t)2;
    seq_t cumulative = (seq_t)0;
    idx_t length = 0;

    /* Find the digit length of the nth positive number */
    for (idx_t d = 1; ; ++d)
    {
        seq_t count_len = choose(d + (idx_t)7, d); /* C(d+7, d) */
        if (cumulative + count_len > (seq_t)rem_idx)
        {
            length = d;
            break;
        }
        cumulative += count_len;
    }

    /* Offset within numbers having 'length' digits */
    seq_t offset = (seq_t)rem_idx - cumulative;

    /* Unrank the offset to the nondecreasing digit sequence */
    uint8_t digits[length];
    uint8_t last_digit = 1;
    for (idx_t pos = 0; pos < length; ++pos)
    {
        for (uint8_t digit = last_digit; digit <= 8; ++digit)
        {
            idx_t remaining_positions = length - pos - (idx_t)1;
            seq_t count;
            if (remaining_positions == (idx_t)0)
            {
                count = (seq_t)1;
            }
            else
            {
                idx_t options = (idx_t)(8 - digit + 1);
                count = choose(options + remaining_positions - (idx_t)1,
                               remaining_positions);
            }

            if (offset < count)
            {
                digits[pos] = digit;
                last_digit   = digit;
                break;
            }
            else
            {
                offset -= count;
            }
        }
    }

    /* Convert the base‑9 digit sequence to decimal */
    seq_t value = (seq_t)0;
    for (idx_t i = 0; i < length; ++i)
    {
        value = value * (seq_t)9 + (seq_t)digits[i];
    }
    return value;
}
