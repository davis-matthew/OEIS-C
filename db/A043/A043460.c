/*
 * A043460  Numbers having four 1's in base 9.
 * https://oeis.org/A043460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th smallest positive integer whose base‑9
 *            representation contains exactly four 1's
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* multiply using exponentiation by squaring */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

/* binomial coefficient C(n, k) for small k (k <= 4 here) */
static seq_t choose(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
        res = res * (seq_t)(n - i + (idx_t)1) / (seq_t)i;
    return res;
}

/* number of m‑digit base‑9 integers with exactly four 1's
   (m >= 4).  */
static seq_t count_length_m(idx_t m)
{
    if (m < (idx_t)4) return (seq_t)0;
    idx_t r = m - (idx_t)1;                     /* positions below the most significant one */

    /* case 1: most significant digit is 1 */
    seq_t cnt = choose(r, (idx_t)3) * pow_seq((seq_t)8, r - (idx_t)3);

    /* case 2: most significant digit is 2..8 */
    if (r >= (idx_t)4)
        cnt += (seq_t)7 * choose(r, (idx_t)4) * pow_seq((seq_t)8, r - (idx_t)4);

    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* find length m such that the n-th number lies among the m‑digit numbers */
    idx_t cumulative = (idx_t)0;
    idx_t m = (idx_t)4;
    while (true)
    {
        seq_t cnt_m = count_length_m(m);
        if (cnt_m >= (seq_t)(n - cumulative)) break;
        cumulative += (idx_t)cnt_m;
        ++m;
    }

    seq_t k = (seq_t)(n - cumulative);     /* 1‑based index within length m */

    seq_t result = (seq_t)0;
    idx_t ones_left = (idx_t)4;

    /* iterate over positions from most significant to least */
    for (idx_t pos = m; pos-- > (idx_t)0; )
    {
        int start = (pos == m - (idx_t)1) ? 1 : 0;   /* msd cannot be 0 */
        for (int d = start; d <= 8; ++d)
        {
            idx_t ones_after = ones_left - ((d == 1) ? (idx_t)1 : (idx_t)0);
            idx_t rem = pos;                         /* positions remaining after this digit */

            if (ones_after > rem || ones_after > (idx_t)4 || ones_after > (idx_t)rem)
                continue;

            seq_t ways = choose(rem, ones_after);
            if (rem > ones_after)
                ways *= pow_seq((seq_t)8, rem - ones_after);

            if (k <= ways)
            {
                result = result * (seq_t)9 + (seq_t)d;
                ones_left = ones_after;
                break;
            }
            else
                k -= ways;
        }
    }
    return result;
}
