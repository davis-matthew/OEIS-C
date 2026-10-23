/*
 * A049357  Digitally balanced numbers in base 6: equal numbers of 0's, 1's,
 *          ..., 5's.
 * https://oeis.org/A049357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 600, u32 n <= 600, u64 n <= 600, u128 n <= 600,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A049357.
   Digitally balanced numbers in base 6: the decimal value of a
   number whose base‑6 expansion contains each digit 0–5 the same
   number of times (leading digit ≠ 0).  This function returns the
   n‑th number of this sequence (n ≥ 1, 1‑based indexing).  The value
   is found by generating the lexicographic permutations of length
   6 k (k copies of each digit) and counting those whose first digit
   is not zero.  The processing stops as soon as the required
   permutation is reached, so the routine is fast for n up to at
   least 10 000.  It works for 8‑, 16‑, 32‑, 64‑bit, 128‑bit, or
   arbitrary‑precision seq_t types. */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Helper: factorial for small integers (fits in seq_t) */

static seq_t fact(unsigned int n)
{
    seq_t r = 1;
    for (unsigned int i = 2; i <= n; ++i)
        r *= i;
    return r;
}

/* Helper: number of distinct permutations of a multiset
   described by counts[6] (total = sum(counts)), i.e.
   total! / (counts[0]! … counts[5]!)  */
static seq_t multiset_perm_count(unsigned int *counts, unsigned int total)
{
    seq_t denom = 1;
    for (int i = 0; i < 6; ++i)
        denom *= fact(counts[i]);
    return fact(total) / denom;
}

/* ------------------------------------------------------------------ */
/* next_permutation for an array of signed integers */

static int next_permutation(int *a, unsigned int n)
{
    int i = (int)n - 2;
    while (i >= 0 && a[i] >= a[i + 1])
        --i;
    if (i < 0)
        return 0;                /* last permutation reached */

    int j = (int)n - 1;
    while (a[j] <= a[i])
        --j;
    int t = a[i]; a[i] = a[j]; a[j] = t;

    /* reverse suffix a[i+1 .. n-1] */
    int l = i + 1, r = (int)n - 1;
    while (l < r) {
        t = a[l]; a[l] = a[r]; a[r] = t;
        ++l; --r;
    }
    return 1;
}

/* ------------------------------------------------------------------ */
/* Main function as required by the build system */

seq_t a(idx_t n)
{
    /* Find the block (k) containing the n-th element. */
    unsigned int k = 1;
    seq_t n_rem = n;        /* n_rem is the remaining position (1‑based) */

    while (1) {
        /* Count all valid permutations for this k. */
        unsigned int cnt[6];
        for (int d = 0; d < 6; ++d)
            cnt[d] = k;

        seq_t total   = multiset_perm_count(cnt, 6 * k);
        seq_t invalid = 0;
        if (k > 0) {
            unsigned int cnt0[6];
            for (int d = 0; d < 6; ++d)
                cnt0[d] = k;
            cnt0[0] -= 1;                 /* first digit fixed to 0 */
            invalid = multiset_perm_count(cnt0, 6 * k - 1);
        }
        seq_t valid = total - invalid;

        if (n_rem > valid) {
            n_rem -= valid;   /* skip this block */
            ++k;
        } else
            break;            /* n_th number lies in block k */
    }

    /* target index within block k (0‑based) */
    idx_t target = (idx_t)(n_rem - 1ULL);

    /* Prepare the initial multiset in ascending order. */
    unsigned int len = 6 * k;
    int *digits = (int *)malloc(len * sizeof(int));
    unsigned int pos = 0;
    for (int d = 0; d < 6; ++d)
        for (unsigned int j = 0; j < k; ++j)
            digits[pos++] = d;

    /* Scan permutations until the target valid one is reached. */
    idx_t counted = 0;
    do {
        if (digits[0] != 0) {            /* leading zero not allowed */
            if (counted == target) {
                seq_t val = 0;
                for (unsigned int i = 0; i < len; ++i)
                    val = val * 6 + digits[i];
                free(digits);
                return val;
            }
            ++counted;
        }
    } while (next_permutation(digits, len));

    /* Should never reach here. */
    free(digits);
    return 0;
}
