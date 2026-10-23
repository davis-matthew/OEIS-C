/*
 * A043657  Numbers whose base-13 representation has exactly 2 runs.
 * https://oeis.org/A043657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Numbers whose base‑13 representation has exactly two runs.
 * a(n) generates the n-th such number (a(1)=13, a(2)=15, …).
 */
#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort on seq_t values */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Ensure that an array has room for at least `need` elements. */
static void ensure_capacity(size_t need, size_t *cap, seq_t **arr)
{
    if (*cap >= need) return;
    size_t newcap = (*cap ? *cap * 2 : 32);
    if (newcap < need) newcap = need;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* Store powers of 13 and the corresponding (13^k‑1)/12 values.
   These arrays are expanded as needed. */
static seq_t *pow13 = NULL;
static seq_t *S13   = NULL;
static size_t powCap = 0;

static void ensure_pow(size_t len)
{
    while (powCap <= len) {
        seq_t v = (powCap == 0) ? 1 : pow13[powCap - 1] * (seq_t)13;
        pow13 = (seq_t *)realloc(pow13, (powCap + 1) * sizeof(seq_t));
        S13   = (seq_t *)realloc(S13,   (powCap + 1) * sizeof(seq_t));
        pow13[powCap] = v;
        S13[powCap]   = (v - (seq_t)1) / (seq_t)12;
        ++powCap;
    }
}

/* Generate all numbers of base‑13 length `len` (len >= 2) and append
   them to the global cache.  The numbers produced by a particular
   length are sorted before being merged. */
static void generate_len(idx_t len, seq_t **cache, size_t *cap, size_t *sz)
{
    /* Temporary buffer for numbers of this length */
    size_t localCap = 0, localSz = 0;
    seq_t *local = NULL;

    for (idx_t k = 1; k < len; ++k) {          /* first run length */
        idx_t l = len - k;                    /* second run length */
        seq_t pow_k = pow13[k];
        seq_t S_k   = S13[k];
        seq_t pow_l = pow13[l];
        seq_t S_l   = S13[l];

        for (int d1 = 1; d1 <= 12; ++d1) {
            seq_t part1 = (seq_t)d1 * pow_l * S_k;
            for (int d2 = 0; d2 <= 12; ++d2) {
                if (d2 == d1) continue;
                seq_t val = part1 + (seq_t)d2 * S_l;

                if (localSz == localCap) {
                    localCap = (localCap ? localCap * 2 : 32);
                    local = (seq_t *)realloc(local, localCap * sizeof(seq_t));
                }
                local[localSz++] = val;
            }
        }
    }

    /* Sort the numbers of this length */
    qsort(local, localSz, sizeof(seq_t), cmp_seq_t);

    /* Append them to the global cache */
    ensure_capacity(*sz + localSz, cap, cache);
    for (size_t i = 0; i < localSz; ++i)
        (*cache)[*sz + i] = local[i];
    *sz += localSz;

    free(local);
}

/* The main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0, sz = 0;
    static idx_t nextLen = 2;   /* smallest length yielding two runs */

    if (n == 0) return 0;  /* undefined for n < 1 */

    /* Generate additional lengths until we have at least n terms. */
    while (sz < n) {
        ensure_pow(nextLen - 1);   /* make power tables available */
        generate_len(nextLen, &cache, &cap, &sz);
        ++nextLen;
    }
    return cache[n - 1];
}
