/*
 * A050061  a(n) = a(n-1) + a(m) for n >= 4, where m = 2^(p+1) + 2 - n and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1, a(2) = 3, and a(3) = 2.
 * https://oeis.org/A050061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 72, u32 n <= 515, u64 n <= 4339,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=3, a(3)=2.
   For n≥4: a(n)=a(n-1)+a(m) where
   m = 2^(p+1)+2-n and p=floor(log2(n-2)). */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation table (1‑based index) */
    static seq_t *seq = NULL;      /* array of terms */
    static size_t cap = 0;         /* allocated size (number of slots) */
    static idx_t sz = 0;           /* highest index computed so far */

    /* base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)2;

    /* initialise the table on first use */
    if (sz == 0) {
        cap = 4;
        seq = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        seq[0] = 0;                 /* unused */
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)3;
        seq[3] = (seq_t)2;
        sz   = 3;
    }

    /* grow the table if necessary */
    if ((size_t)n + 1U > cap) {
        size_t new_cap = cap ? cap << 1 : 4;
        if (new_cap < (size_t)n + 1U) new_cap = (size_t)n + 1U;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = sz + 1; i <= n; ++i) {
        /* p = floor(log2(i-2)) */
        unsigned long long t   = (unsigned long long)i - 2ULL;   /* i≥4 → t≥2 */
        unsigned long long p   = 63ULL - __builtin_clzll(t);      /* floor log2(t) */
        unsigned long long pow2 = 1ULL << (p + 1ULL);            /* 2^(p+1)   */
        unsigned long long m_i  = pow2 + 2ULL - (unsigned long long)i;
        idx_t m = (idx_t)m_i;                                   /* m≥1 and <i   */
        seq[i] = seq[i - 1] + seq[m];
    }
    sz = n;
    return seq[n];
}
