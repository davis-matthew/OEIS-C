/*
 * A043506  Numbers having two 4's in base 10.
 * https://oeis.org/A043506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose decimal representation
 *            contains exactly two digits 4 (OEIS A043506). The function
 *            enumerates the numbers in increasing order
 * Range:     u8 n <= 3, u16 n <= 5719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for bool and size_t */
#include <stdbool.h>

static idx_t pow9_arr[512];
static idx_t pow9_len = 0;

/* Ensure powers of 9 up to exponent 'need' are calculated. */
static void ensure_pow9(idx_t need)
{
    if (need < pow9_len)
        return;
    for (idx_t i = pow9_len; i <= need; ++i) {
        if (i == 0)
            pow9_arr[0] = 1;
        else
            pow9_arr[i] = pow9_arr[i - 1] * (idx_t)9;
    }
    pow9_len = need + 1;
}

/* Binomial coefficient C(n,k) for small n (return type idx_t). */
static idx_t comb(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;            /* exact division */
    }
    return res;
}

/* Number of completions for the remaining positions.
   rem  : number of positions left
   rem4 : number of 4's still to place
   Return type idx_t to avoid overflow for counts larger than seq_t. */
static idx_t completions(idx_t rem, idx_t rem4)
{
    if (rem < rem4)
        return 0;
    ensure_pow9(rem);
    if (rem4 == 0)
        return pow9_arr[rem];                               /* 9^rem possibilities */
    return comb(rem, rem4) * pow9_arr[rem - rem4];
}

/* Count of admissible numbers of length d (d ≥ 2). Result fits in idx_t. */
static idx_t count_len(idx_t d)
{
    if (d < 2)
        return 0;
    if (d == 2)
        return 1;                                           /* only 44 */
    ensure_pow9(d - 2);
    idx_t c1 = (idx_t)(d - 1) * pow9_arr[d - 2];             /* first digit = 4 */
    ensure_pow9(d - 3);
    idx_t c2 = 8 * comb(d - 1, 2) * pow9_arr[d - 3];         /* first ≠4 but ≠0 */
    return c1 + c2;
}

/* ---------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Find the digit length d of the n-th number. */
    idx_t cumulative = 0;
    idx_t d = 2;
    while (1) {
        idx_t cnt_len = count_len(d);
        if (cumulative + cnt_len >= n)
            break;
        cumulative += cnt_len;
        ++d;
    }

    /* Rank within numbers of length d (1‑based). */
    idx_t k = n - cumulative;          /* still 1‑based */

    seq_t result = 0;
    idx_t rem4 = 2;                    /* two 4's to place */

    for (idx_t pos = 0; pos < d; ++pos) {
        idx_t rem_pos = d - pos - 1;
        for (int cand = 0; cand < 10; ++cand) {
            if (pos == 0 && cand == 0)             /* leading zero forbidden */
                continue;
            bool is4 = (cand == 4);
            if (is4 && rem4 == 0)                  /* cannot place more 4's */
                continue;
            idx_t cnt = is4 ?
                completions(rem_pos, rem4 - 1) :
                completions(rem_pos, rem4);
            if (k <= cnt) {                        /* this digit is chosen */
                result = result * (seq_t)10 + (seq_t)cand;
                if (is4)
                    --rem4;
                break;
            } else {
                k -= cnt;
            }
        }
    }

    return result;
}
