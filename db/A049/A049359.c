/*
 * A049359  Digitally balanced numbers in base 8: equal numbers of 0's, 1's,
 *          ..., 7's.
 * https://oeis.org/A049359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th smallest positive integer whose base‑8 *
 *            representation contains each of the digits 0–7 an equal number
 *            of times * (leading zero not allowed)
 * Range:     u8 none, u16 none, u32 n <= 720, u64 n <= 720, u128 n <= 720,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the helper functions alloc_seq_array and free_seq_array are
 * provided by the build (see ../utils/alloc_seq_array.h and
 * ../utils/free_seq_array.h). */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Return the count of 8k‑digit base‑8 numbers that use each digit
 * exactly k times, with no leading zero. */
static seq_t count_of_k(idx_t k)
{
    if (k == 0)
        return 0;

    idx_t L = 8 * k;
    /* factorial(0..L) */
    seq_t *fact = alloc_seq_array((size_t)(L + 1));
    if (!fact)
        return 0;
    fact[0] = 1;
    for (idx_t i = 1; i <= L; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t total   = fact[L];
    seq_t fact_k  = fact[k];
    seq_t denom   = 1;
    for (int i = 0; i < 8; ++i)
        denom *= fact_k;
    seq_t count = total / denom;

    if (k >= 1) {
        seq_t leading_total = fact[L - 1];
        seq_t denom2 = fact[k - 1];
        for (int i = 0; i < 7; ++i)
            denom2 *= fact_k;
        seq_t leading = leading_total / denom2;
        count -= leading;
    }

    free_seq_array(fact);
    return count;
}

/* Return the n-th element of the sequence.  The index is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1 */

    /* Find block size k such that the nth number lies in it. */
    idx_t k = 1;
    seq_t cumulative = 0;
    seq_t block_cnt;

    while (1) {
        block_cnt = count_of_k(k);
        if (cumulative + block_cnt >= (seq_t)n)
            break;
        cumulative += block_cnt;
        ++k;
    }

    /* Position within the block (0‑based). */
    seq_t r = (seq_t)n - cumulative - 1;

    /* Prepare factorial array up to 8k. */
    idx_t L = 8 * k;
    seq_t *fact = alloc_seq_array((size_t)(L + 1));
    if (!fact)
        return 0;
    fact[0] = 1;
    for (idx_t i = 1; i <= L; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Unranking of the r‑th multiset permutation. */
    unsigned char *digits = (unsigned char*)malloc((size_t)L);
    if (!digits) {
        free_seq_array(fact);
        return 0;
    }

    idx_t cnt[8];
    for (int d = 0; d < 8; ++d)
        cnt[d] = k;
    idx_t remaining = L;

    for (idx_t pos = 0; pos < L; ++pos) {
        for (int d = 0; d < 8; ++d) {
            if (pos == 0 && d == 0)
                continue;
            if (cnt[d] == 0)
                continue;

            /* Assume we place digit d. */
            cnt[d]--; remaining--;
            seq_t perms = fact[remaining];
            for (int dd = 0; dd < 8; ++dd)
                perms /= fact[cnt[dd]];
            /* Undo assumption. */
            cnt[d]++; remaining++;

            if (perms > r) {
                digits[pos] = (unsigned char)d;
                cnt[d]--; remaining--;
                break;
            } else {
                r -= perms;
            }
        }
    }

    /* Convert the base‑8 digits to a decimal value. */
    seq_t value = 0;
    for (idx_t pos = 0; pos < L; ++pos)
        value = value * (seq_t)8 + (seq_t)digits[pos];

    free(digits);
    free_seq_array(fact);
    return value;
}
