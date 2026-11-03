/*
 * A067004  Number of numbers <= n with same number of divisors as n.
 * https://oeis.org/A067004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of k ≤ n with the same number of divisors as n
 * Range:     u8 n <= 870, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9835 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static state for memoization */
    static idx_t max_n       = 0;                  /* largest n processed */
    static seq_t *divCnt     = NULL;               /* divisor counts 1 .. max_n */
    static seq_t *freq       = NULL;               /* frequency of each tau */
    static size_t freq_size  = 0;                  /* size of freq array */

    if (n < 1) return 0;                           /* undefined for n<1 */

    if (n > max_n)                                 /* recompute up to n */
    {
        /* -------- allocate & clear divisor count array -------- */
        divCnt = mem_resize(seq_t, divCnt, (size_t)(n + 1));
        for (idx_t i = 1; i <= n; ++i) divCnt[i] = 0;

        /* -------- compute divisor counts by simple sieve -------- */
        for (idx_t i = 1; i <= n; ++i)
        {
            for (idx_t j = i; j <= n; j += i)
                ++divCnt[j];
        }

        /* -------- determine maximum divisor count (tau) -------- */
        seq_t maxTau = divCnt[1];
        for (idx_t i = 2; i <= n; ++i)
            if (divCnt[i] > maxTau) maxTau = divCnt[i];

        /* -------- allocate & zero frequency array -------- */
        size_t new_freq_size = (size_t)(maxTau + 1);
        if (freq) mem_free(freq);                      /* discard old table */
        freq = mem_new(seq_t, new_freq_size); /* zeroed */
        for (idx_t i = 1; i <= n; ++i)
            ++freq[(size_t)divCnt[i]];

        /* -------- update static state -------- */
        max_n      = n;
        freq_size  = new_freq_size;
    }

    /* -------- answer for current n -------- */
    seq_t tau_n = divCnt[n];
    return freq[(size_t)tau_n];
}
