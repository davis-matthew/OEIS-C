/*
 * A179933  n replaced by a list of the distinct positive integers that can be
 *          formed with the decimal digits of n.
 * https://oeis.org/A179933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A179933 – Flatten the list of all distinct non‑zero numbers that can be
   be formed with the decimal digits of each integer k, outputting the
   terms in order. */

/* Standard headers needed for all builds. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Comparison function for qsort over seq_t values. */
static int cmp_seq_t(const void *p, const void *q)
{
    seq_t va = *(const seq_t *)p;
    seq_t vb = *(const seq_t *)q;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Comparison function for qsort over int values. */
static int cmp_int(const void *a, const void *b)
{
    return (*(const int *)a - *(const int *)b);
}

/* Compute the next lexicographic permutation of array a of length n. */
static bool next_permutation(int *a, size_t n)
{
    if (n <= 1) return false;
    size_t i = n - 1;
    while (i > 0 && a[i - 1] >= a[i]) --i;
    if (i == 0) return false;
    size_t j = n - 1;
    while (a[j] <= a[i - 1]) --j;
    int tmp = a[i - 1];
    a[i - 1] = a[j];
    a[j] = tmp;
    /* reverse suffix [i, n) */
    size_t l = i, r = n - 1;
    while (l < r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
        ++l;
        --r;
    }
    return true;
}

/* Maximum number of distinct non-zero values that can be produced from
   any k <= 10000. 5 distinct digits → 31 subsets × 120 permutations = 3720.
   4000 is a safe upper bound. */
#define MAX_NUMS 4000

seq_t a(idx_t target)
{
    /* position in the flattened list (1‑based) */
    idx_t pos = 0;

    for (idx_t k = 1; ; ++k) {
        /* extract decimal digits of k into array 'digits' (least significant first) */
        int digits[20];
        size_t d = 0;
        idx_t t = k;
        do {
            digits[d++] = t % 10;
            t /= 10;
        } while (t > 0);

        /* buffer to store all generated numbers for this k */
        seq_t numbers[MAX_NUMS];
        size_t len = 0;

        /* iterate over all non‑empty subsets of the digits */
        size_t total_masks = ((size_t)1 << d);
        for (size_t mask = 1; mask < total_masks; ++mask) {
            /* collect digits belonging to this subset into 'sub' */
            int sub[20];
            size_t sd = 0;
            for (size_t i = 0; i < d; ++i)
                if (mask & ((size_t)1 << i))
                    sub[sd++] = digits[i];

            /* sort to start generating unique permutations */
            qsort(sub, sd, sizeof(int), cmp_int);

            /* enumerate all distinct permutations of the subset */
            do {
                /* convert digit sequence to integer value */
                seq_t val = 0;
                for (size_t j = 0; j < sd; ++j)
                    val = val * 10 + (seq_t)sub[j];
                if (val == 0) continue;          /* exclude zero */

                /* store the value */
                /* it is safe because len < MAX_NUMS by construction */
                numbers[len++] = val;
            } while (next_permutation(sub, sd));
        }

        /* sort and deduplicate numbers for this k */
        qsort(numbers, len, sizeof(seq_t), cmp_seq_t);
        size_t uniq = 0;
        for (size_t i = 0; i < len; ++i) {
            if (uniq == 0 || numbers[i] != numbers[uniq - 1]) {
                numbers[uniq++] = numbers[i];
            }
        }
        len = uniq;

        /* emit the numbers, checking whether we hit the target index */
        for (size_t i = 0; i < len; ++i) {
            if (++pos == target) {
                return numbers[i];
            }
        }
    }
}
