/*
 * A043526  Numbers having two 9's in base 10.
 * https://oeis.org/A043526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3056, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043526: numbers having exactly two 9's in base 10.
   The function returns the n‑th such number (with n ≥ 1) by
   constructing the number directly from its digit representation
   rather than by searching. */

/* ------------------------------------------------------------------
   helper routines
   ------------------------------------------------------------------ */
#include <stdint.h>
#include <stdbool.h>

/* integer power – works for all seq_t types (uint8_t … unsigned __int128) */
static seq_t pow_uint(uint64_t base, idx_t exp)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        r *= (seq_t)base;
    return r;
}

/* binomial coefficient – exact for small n (here n ≤ 30)   */
static seq_t binomial_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        r *= (seq_t)(n - k + i);
        r /= (seq_t)i;
    }
    return r;
}

/* number of completions for the remaining positions with a given
   remaining count of 9's.  Positions left may be zero. */
static seq_t count_completions(idx_t pos_left, idx_t rem9)
{
    if (rem9 > pos_left) return (seq_t)0;
    seq_t choose  = binomial_idx(pos_left, rem9);
    seq_t pow9   = pow_uint(9, pos_left - rem9);
    return choose * pow9;
}

/* count of all numbers of length L with exactly two 9's
   and with a non‑zero first digit. */
static seq_t count_two_nines(idx_t L)
{
    if (L < 2) return (seq_t)0;
    seq_t total = (seq_t)0;
    /* first digit NOT a 9 */
    if (L >= 3) {
        seq_t choose   = binomial_idx(L - 1, 2);
        seq_t pow9     = pow_uint(9, L - 3);   /* <--- correct base 9 */
        total += (seq_t)8 * choose * pow9;
    }
    /* first digit IS a 9 */
    {
        seq_t choose   = binomial_idx(L - 1, 1);
        seq_t pow9     = pow_uint(9, L - 2);
        total += choose * pow9;
    }
    return total;
}

/* ------------------------------------------------------------------
   main sequence routine
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n is 1‑based, n ≥ 1 */
    idx_t rem = n;       /* remaining rank inside current length block */
    idx_t L   = 2;       /* minimal length (99) */

    /* find the length block that contains the n‑th number */
    while (true) {
        seq_t cnt = count_two_nines(L);
        if ((seq_t)rem <= cnt) break;
        rem -= (idx_t)cnt;
        ++L;
    }

    /* build the rem‑th (1‑based) number of length L */
    uint8_t digits[64] = {0};
    idx_t   rem9 = 2;

    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t   pos_left = L - pos - 1;
        unsigned start   = (pos == 0) ? 1 : 0;   /* first digit non‑zero */
        for (int d = start; d <= 9; ++d) {
            if (d == 9 && rem9 == 0) continue;
            idx_t new_rem9 = rem9 - (d == 9 ? 1 : 0);
            seq_t comp     = count_completions(pos_left, new_rem9);
            if ((seq_t)rem > comp) {
                rem -= (idx_t)comp;
                continue;
            }
            digits[pos] = (uint8_t)d;
            rem9 = new_rem9;
            break;
        }
    }

    /* assemble the final integer value */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < L; ++i)
        result = result * (seq_t)10 + (seq_t)digits[i];

    return result;
}
