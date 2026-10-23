/*
 * A045303  Numbers whose base-5 representation contains exactly two 3's and
 *          three 4's.
 * https://oeis.org/A045303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth integer whose base‑5 representation
   contains exactly two 3's and three 4's.
   The algorithm enumerates lengths, uses combinatorial counts,
   and builds the nth number digit by digit. */

#include <stdint.h>
#include <stddef.h>

/* small combinatorics used only for very small arguments */
static seq_t choose(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;          /* exact division */
    }
    return res;
}

static seq_t pow3(idx_t e)
{
    seq_t p = (seq_t)1;
    for (idx_t i = (idx_t)0; i < e; ++i) p = p * (seq_t)3;
    return p;
}

/* number of completions for given remaining positions and needed
   counts of digits 3 and 4                                  */
static seq_t count_remaining(idx_t remaining, idx_t rem3, idx_t rem4)
{
    if (rem3 > remaining || rem4 > remaining) return (seq_t)0;
    if (rem3 + rem4 > remaining) return (seq_t)0;

    seq_t ways = choose(remaining, rem3);
    ways = ways * choose(remaining - rem3, rem4);
    ways = ways * pow3(remaining - rem3 - rem4);
    return ways;
}

/* total number of admissible numbers of length L (no leading zero) */
static seq_t total_count_len(idx_t L)
{
    /* total without leading‑zero restriction */
    seq_t total = choose(L, (idx_t)5);
    total = total * (seq_t)10;          /* 10 ways to assign two 3's and three 4's */
    total = total * pow3(L - (idx_t)5);

    /* subtract those with leading 0 */
    if (L == (idx_t)5) return total;    /* no other positions */
    seq_t bad = choose(L - (idx_t)1, (idx_t)5);
    bad = bad * (seq_t)10;
    bad = bad * pow3(L - (idx_t)6);
    return total - bad;
}

/* nth term (n >= 1) */
seq_t a(idx_t n)
{
    /* 1. Find length L containing the nth number */
    idx_t L = (idx_t)5;
    idx_t cumulative = (idx_t)0;
    seq_t cumulative_seq = (seq_t)0;
    seq_t cnt;
    for (;;) {
        cnt = total_count_len(L);
        cumulative_seq = cumulative_seq + cnt;
        cumulative += (idx_t)cnt;            /* cnt fits in idx_t for our sizes */
        if (cumulative >= n) break;
        ++L;
    }

    /* 2. Position within numbers of length L */
    idx_t k = n - cumulative + (idx_t)cnt;   /* 1‑based index inside this length */

    /* 3. Build digits from most significant to least */
    uint8_t digits[64];                      /* L will never exceed 64 for requested n */
    idx_t rem3 = (idx_t)2;
    idx_t rem4 = (idx_t)3;

    for (idx_t pos = (idx_t)0; pos < L; ++pos) {
        idx_t remaining = L - pos - (idx_t)1;
        uint8_t start = (pos == (idx_t)0) ? (uint8_t)1 : (uint8_t)0;
        for (uint8_t d = start; d <= (uint8_t)4; ++d) {
            if (d == (uint8_t)3 && rem3 == (idx_t)0) continue;
            if (d == (uint8_t)4 && rem4 == (idx_t)0) continue;

            idx_t newRem3 = rem3;
            idx_t newRem4 = rem4;
            if (d == (uint8_t)3) --newRem3;
            else if (d == (uint8_t)4) --newRem4;

            seq_t ways = count_remaining(remaining, newRem3, newRem4);
            seq_t k_seq = (seq_t)k;

            if (ways >= k_seq) {
                digits[pos] = d;
                rem3 = newRem3;
                rem4 = newRem4;
                break;
            } else {
                k -= (idx_t)ways;
            }
        }
    }

    /* 4. Convert digit array to value */
    seq_t val = (seq_t)0;
    for (idx_t i = (idx_t)0; i < L; ++i) {
        val = val * (seq_t)5 + (seq_t)digits[i];
    }
    return val;
}
