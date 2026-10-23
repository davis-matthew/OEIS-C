/*
 * A048260  The sum of 2 (not necessarily distinct) abundant numbers.
 * https://oeis.org/A048260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A048260: the nth smallest sum of two abundant
 * numbers.  For n > 18705 the sequence follows the OEIS linear formula  
 * a(n) = n + 1456.  For n ≤ 18705 the term is determined by sifting all sums
 * of two abundant numbers up to the largest known missing integer 20161 and
 * storing the resulting sorted list of representable numbers.  Subsequent
 * calls are answered from this table or from the formula. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static data shared among calls */
    static bool initialized = false;
    static idx_t *sums = NULL;     /* sorted list of representable numbers */
    static size_t seq_len = 0;      /* number of entries in sums */

    if (!initialized) {
        /* The largest integer that cannot be expressed as a sum of two abundant
         * numbers is 20161.  All integers > 20161 are expressible.  
         * Therefore a(n)=n+1456 for n>18705, because the 18706th term is 20162.
         */
        const idx_t LIMIT = 20161;   /* inclusive upper bound for sieving */

        /* 1. Sum of proper divisors for each number up to LIMIT. */
        idx_t *div_sum = (idx_t *)calloc(LIMIT + 1, sizeof(idx_t));
        if (!div_sum) abort();   /* allocation failed */
        for (idx_t i = 1; i <= LIMIT / 2; ++i) {
            for (idx_t j = i * 2; j <= LIMIT; j += i)
                div_sum[j] += i;
        }

        /* 2. Collect abundant numbers ≤ LIMIT. */
        idx_t *abunds = NULL;
        size_t abcnt = 0, abcap = 0;
        for (idx_t i = 1; i <= LIMIT; ++i) {
            if (div_sum[i] > i) {
                if (abcnt == abcap) {
                    abcap = (abcap == 0) ? 64 : abcap * 2;
                    idx_t *tmp = (idx_t *)realloc(abunds, abcap * sizeof(idx_t));
                    if (!tmp) abort();
                    abunds = tmp;
                }
                abunds[abcnt++] = i;
            }
        }
        free(div_sum);

        /* 3. Boolean array marking sums of two abundant numbers ≤ LIMIT. */
        bool *rep = (bool *)calloc(LIMIT + 1, sizeof(bool));
        if (!rep) abort();
        for (size_t i = 0; i < abcnt; ++i) {
            idx_t a_val = abunds[i];
            for (size_t j = i; j < abcnt; ++j) {
                idx_t s = a_val + abunds[j];
                if (s > LIMIT) break;
                rep[(size_t)s] = true;
            }
        }

        /* 4. Gather representable numbers in ascending order. */
        idx_t *tmp = (idx_t *)malloc((LIMIT + 1) * sizeof(idx_t));
        if (!tmp) abort();
        size_t count = 0;
        for (idx_t v = 1; v <= LIMIT; ++v) {
            if (rep[(size_t)v]) tmp[count++] = v;
        }
        free(rep);
        free(abunds);

        /* 5. Store the list in the static array. */
        sums = (idx_t *)realloc(tmp, count * sizeof(idx_t));
        if (!sums) abort();
        seq_len = count;        /* Should be 18705 for LIMIT = 20161 */
        initialized = true;
    }

    /* 6. Return the requested term. */
    if (n <= seq_len) {
        return (seq_t)sums[(size_t)n - 1];
    } else {
        return ((seq_t)n) + (seq_t)1456;
    }
}
