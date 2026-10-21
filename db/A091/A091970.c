/*
 * A091970  a(1) = 0; for n>1, find largest integer k such that the word
 *          a(1)a(2)...a(n-1) is of the form xy^k for words x and y (where y
 *          has positive length), i.e., k = the maximal number of repeating
 *          blocks at the end of the sequence so far; then a(n) = floor(k/2).
 * https://oeis.org/A091970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 3000, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A091970
 * a(1) = 0; for n>1, let k be the maximum integer such that
 * the word a(1) a(2) … a(n‑1) is of the form x y^k (y has positive length).
 * Then a(n) = floor(k/2).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* static storage for the sequence and its reverse-prefix-function */
static seq_t *seq_cache = NULL;         /* a(1) … a(len)  */
static size_t    seq_len  = 0;           /* number of terms cached */
static size_t    seq_cap  = 0;           /* allocated capacity */

/* array for the prefix function of the reversed word */
static int   *pi_rev   = NULL;
static size_t  pi_cap  = 0;

/* Ensure enough capacity for the sequence cache */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 16;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_cache, newcap * sizeof(seq_t));
    if (!tmp) abort();           /* memory error */
    seq_cache = tmp;
    seq_cap   = newcap;
}

/* Ensure enough capacity for the prefix‑function array */
static void ensure_pi_capacity(size_t needed)
{
    if (needed <= pi_cap) return;
    size_t newcap = pi_cap ? pi_cap * 2 : 16;
    while (newcap < needed) newcap <<= 1;
    int *tmp = (int*)realloc(pi_rev, newcap * sizeof(int));
    if (!tmp) abort();
    pi_rev = tmp;
    pi_cap = newcap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, safe guard */
    /* Extend cached values up to n */
    while (seq_len < n)
    {
        size_t m = seq_len;          /* number of terms already stored */

        seq_t val;
        if (m == 0)
        {
            val = 0;                  /* a(1) = 0 */
        }
        else
        {
            /* Build prefix function of the reversed word of length m */
            ensure_pi_capacity(m);
            pi_rev[0] = 0;
            int j = 0;
            for (size_t i = 1; i < m; ++i)
            {
                int cur = (int)seq_cache[m - 1 - i];
                while (j > 0 && cur != (int)seq_cache[m - 1 - j])
                    j = pi_rev[j - 1];
                if (cur == (int)seq_cache[m - 1 - j]) ++j;
                pi_rev[i] = j;
            }

            /* Find the maximum k among all suffixes */
            int max_k = 1;              /* at least 1 (suffix of length 1) */
            for (size_t i = 0; i < m; ++i)
            {
                size_t L = i + 1;            /* prefix length in reversed word */
                int   p = (int)L - pi_rev[i];
                int   k = (L % (size_t)p == 0) ? (int)(L / (size_t)p) : 1;
                if (k > max_k) max_k = k;
                if ((size_t)max_k == m) break; /* cannot exceed m */
            }
            val = (seq_t)(max_k / 2);
        }

        /* Store the computed term */
        ensure_seq_capacity(seq_len + 1);
        seq_cache[seq_len++] = val;
    }
    return seq_cache[n - 1];
}
