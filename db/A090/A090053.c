/*
 * A090053  Numbers divisible by the number formed when their digits are
 *          sorted in ascending order, excluding trivial cases.
 * https://oeis.org/A090053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate terms by checking each integer sequentially,
   skipping trivial cases and storing the results for future calls. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t last_checked = 0;

static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t newcap = terms_cap ? terms_cap * 2 : 16;
        terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        /* In a real program error handling would be added here. */
        terms_cap = newcap;
    }
    terms[terms_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)        /* undefined for offset 1, return zero safely */
        return (seq_t)0;

    while (terms_len < n) {
        idx_t k = ++last_checked;
        /* Extract digits of k into an array (least significant first). */
        int digs[32];
        int nd = 0;
        idx_t tmp = k;
        while (tmp) {
            digs[nd++] = (int)(tmp % 10);
            tmp /= 10;
        }
        if (nd == 0) {          /* k == 0, should not happen for k>=1 */
            append_term((seq_t)k);
            continue;
        }

        /* Compute digits array with most significant first for easy checks. */
        int ms[32];
        for (int i = 0; i < nd; ++i)
            ms[i] = digs[nd - 1 - i];

        /* Count trailing zeros (least significant zeros). */
        int tz = 0;
        while (tz < nd && ms[nd - 1 - tz] == 0)
            ++tz;

        /* Case (1): digits already in ascending order. */
        bool asc_orig = true;
        for (int i = 1; i < nd; ++i)
            if (ms[i] < ms[i - 1]) {
                asc_orig = false;
                break;
            }
        if (asc_orig)
            continue;          /* trivial case 1 */

        /* Case (2): no trailing zeros, but digits other than trailing zeros
           are in ascending order. */
        if (tz > 0) {
            bool asc_no_tz = true;
            for (int i = 1; i < nd - tz; ++i)
                if (ms[i] < ms[i - 1]) {
                    asc_no_tz = false;
                    break;
                }
            if (asc_no_tz)
                continue;          /* trivial case 2 */
        }

        /* Count digits 0-9. */
        int cnt[10] = {0};
        for (int i = 0; i < nd; ++i)
            ++cnt[digs[i]];

        /* Compute ASort(k): integer formed by non‑zero digits sorted ascending. */
        seq_t d = 0;
        for (int digit = 1; digit <= 9; ++digit)
            for (int i = 0; i < cnt[digit]; ++i)
                d = d * 10 + digit;
        if (d == 0)        /* should not happen for k>0 */
            continue;

        seq_t num = (seq_t)k;
        if (num % d == 0)
            append_term(num);
    }

    return terms[n - 1];
}
