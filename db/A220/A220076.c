/*
 * A220076  Numbers with exactly six distinct base-10 digits.
 * https://oeis.org/A220076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A220076: numbers having exactly six distinct decimal digits.
   The code computes a(n) by combinatorial enumeration of L‑digit numbers
   with exactly six distinct digits, using inclusion–exclusion for
   counting completions of the number. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow_int.h"   /* pow_int(seq_t base, idx_t exp) */

/* Small integer binomial coefficient, returned as seq_t. */
static seq_t binom_int(int n, int k)
{
    if (k < 0 || k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (int i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

/* Inclusion–exclusion count: number of length 'rem' strings over 6
   digits that contain all k specified digits at least once. */
static seq_t f_count(int rem, int k)
{
    seq_t res = (seq_t)0;
    for (int i = 0; i <= k; ++i) {
        seq_t term = binom_int(k, i) *
                     pow_int((seq_t)(6 - i), (idx_t)rem);
        if ((i & 1) == 0)
            res += term;
        else
            res -= term;
    }
    return res;
}

/* Count of L‑digit numbers (leading digit ≠ 0) with exactly six distinct
   decimal digits.  Formula:
       count(L) = C(10,6) * f_count(L,6)  –  C(9,5) * f_count(L-1,5)
   (for L ≥ 1). */
static seq_t count_len_L(int L)
{
    seq_t total = binom_int(10, 6) * f_count(L, 6);
    seq_t zero_lead = binom_int(9, 5) * f_count(L - 1, 5);
    return total - zero_lead;
}

/* Given the length L and 1‑based rank (within this length), construct
   the lexicographically smallest number with exactly six distinct digits. */
static seq_t nth_of_len(int L, seq_t rank)
{
    seq_t result = (seq_t)0;
    int mask = 0;       /* bits for digits that have appeared */
    int m = 0;          /* current number of distinct digits used */

    for (int pos = 0; pos < L; ++pos) {
        int rem = L - pos - 1;           /* positions left after this one */
        int start = (pos == 0) ? 1 : 0;  /* no leading zero */

        for (int d = start; d <= 9; ++d) {
            int bit = 1 << d;
            int new_m = m;
            bool add_new = (mask & bit) == 0;
            if (add_new) new_m++;

            int k = 6 - new_m;
            if (k < 0) continue;          /* impossible, too many digits */

            seq_t count;
            if (rem == 0) {
                /* Last position: only one number if we have now exactly 6 digits */
                count = (new_m == 6) ? (seq_t)1 : (seq_t)0;
            } else {
                seq_t ways_mask = binom_int(10 - new_m, k);
                seq_t f_val = f_count(rem, k);
                count = ways_mask * f_val;
            }

            if (count >= rank) {
                /* choose this digit and proceed */
                result = result * (seq_t)10 + (seq_t)d;
                mask |= bit;
                m = new_m;
                break;
            } else {
                rank -= count;
            }
        }
    }
    return result;
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is 1‑based */

    seq_t target = (seq_t)n;
    seq_t cum = (seq_t)0;
    int L = 6;

    while (true) {
        seq_t cnt = count_len_L(L);
        if (cum + cnt >= target)
            break;
        cum += cnt;
        ++L;
    }

    seq_t rank = target - cum;            /* rank within block of length L */
    return nth_of_len(L, rank);
}
