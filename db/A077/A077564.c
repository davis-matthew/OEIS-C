/*
 * A077564  Number of partitions of n into parts with distinct prime
 *          signatures.
 * https://oeis.org/A077564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 143, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 79 terms through n = 197 (2026-11-03)
 * Helpers:   build_spf, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over groups of integers that share the same prime signature.
   Each group may contribute at most one part to a partition of n.  The DP
   accumulates the number of ways to achieve each partial sum.  Complexity
   is roughly O(n^2) with a small constant, fast enough for n ≈ 10000.  */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* external utilities available from the harness */
#include <stddef.h>
#include "../utils/build_spf.h"
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* build a canonical string representation of the sorted list of
   exponents of the prime factorisation of m.
   spf contains the smallest prime factor of each integer ≤ n. */
static void build_signature(idx_t m, idx_t *spf, size_t spf_cnt,
                            char *buf, size_t buf_len)
{
    if (m == 1) {                /* 1 has an empty signature */
        buf[0] = '\0';
        return;
    }

    int exps[32];
    int ecnt = 0;
    idx_t x = m;

    /* factorisation using SPF table */
    while (x > 1) {
        idx_t p = spf[x];
        int cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        exps[ecnt++] = cnt;
    }

    /* sort exponents ascending */
    for (int i = 1; i < ecnt; ++i) {
        int j = i;
        while (j > 0 && exps[j - 1] > exps[j]) {
            int t = exps[j];
            exps[j] = exps[j - 1];
            exps[j - 1] = t;
            --j;
        }
    }

    /* encode the sorted exponents into a string "1-2-1" etc. */
    char *p = buf;
    for (int i = 0; i < ecnt; ++i) {
        int written = snprintf(p, buf_len - (size_t)(p - buf),
                               i == 0 ? "%d" : "-%d", exps[i]);
        if (written < 0) break;   /* error, abort (should not happen) */
        p += written;
    }
    *p = '\0';
}

/* maximum length required for a signature string
   (number of prime exponents is ≤ log₂(n) ≤ 14 for n ≤ 10⁴) */
#define SIG_KEY_LEN 64

/* entry used for sorting numbers by signature */
struct sig_entry {
    idx_t num;                    /* the integer value */
    char key[SIG_KEY_LEN];        /* its prime‑signature key */
};

/* comparison function for qsort (requires explicit casts for C++) */
static int cmp_sig(const void *a, const void *b)
{
    const struct sig_entry *sa = (const struct sig_entry *)a;
    const struct sig_entry *sb = (const struct sig_entry *)b;
    return strcmp(sa->key, sb->key);
}

/* requested function */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* allocate array of SPF up to n */
    idx_t *spf = NULL;
    build_spf(n, &spf);
    if (!spf)
        return (seq_t)0;          /* allocation failure */

    /* number of entries is n (we ignore 0 for simplicity) */
    size_t N = (size_t)n;

    /* allocate array of signature entries */
    struct sig_entry *entries = (struct sig_entry *)
        malloc((N + 1) * sizeof(struct sig_entry));
    if (!entries) {
        free(spf);
        return (seq_t)0;
    }

    /* build entries for numbers 1..n */
    for (size_t i = 1; i <= N; ++i) {
        entries[i].num = (idx_t)i;
        build_signature((idx_t)i, spf, n + 1,
                        entries[i].key, SIG_KEY_LEN);
    }

    free(spf);                  /* SPF no longer needed */

    /* sort entries by signature key, skipping index 0 */
    qsort(entries + 1, N, sizeof(struct sig_entry), cmp_sig);

    /* allocate DP arrays */
    seq_t *dp = alloc_seq_array(N + 1);
    seq_t *add = alloc_seq_array(N + 1);
    if (!dp || !add) {
        free(entries);
        free_seq_array(dp);
        free_seq_array(add);
        return (seq_t)0;
    }

    /* initialise dp[0] = 1, rest 0 */
    for (size_t i = 0; i <= N; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* process each group of identical signatures */
    size_t pos = 1;
    while (pos <= N) {
        /* identify group with same key */
        size_t start = pos;
        const char *group_key = entries[pos].key;
        while (pos <= N && strcmp(entries[pos].key, group_key) == 0)
            ++pos;                     /* pos now points to first after group */

        /* zero the additive array for this group */
        for (size_t i = 0; i <= N; ++i)
            add[i] = (seq_t)0;

        /* for each number in the group, add its contribution */
        for (size_t i = start; i < pos; ++i) {
            idx_t m = entries[i].num;
            for (idx_t sum = m; sum <= n; ++sum) {
                add[(size_t)sum] += dp[(size_t)(sum - m)];
            }
        }

        /* merge the group's contributions into dp */
        for (size_t i = 0; i <= N; ++i)
            dp[i] += add[i];
    }

    seq_t result = dp[(size_t)n];

    /* free temporary storage */
    free_seq_array(dp);
    free_seq_array(add);
    free(entries);

    return result;
}
