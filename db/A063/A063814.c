/*
 * A063814  Growth series for fundamental group of orientable closed surface
 *          of genus 4.
 * https://oeis.org/A063814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=16, a(2)=240, a(3)=3600, a(4)=54000, a(5)=810000,
 *            a(6)=12150000, a(7)=182250000, a(8)=2733749984; for n>8:
 *            a(n)=14*a(n-1)+14*a(n-2)+14*a(n-3)+14*a(n-4)+14*a(n-5)+
 *            14*a(n-6)+14*a(n-7)-a(n-8)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first nine terms directly. */
    if (n <= 8) {
        static const seq_t init[9] = {
            1,            /* a(0) */
            16,           /* a(1) */
            240,          /* a(2) */
            3600,         /* a(3) */
            54000,        /* a(4) */
            810000,       /* a(5) */
            12150000,     /* a(6) */
            182250000,    /* a(7) */
            2733749984    /* a(8) */
        };
        return init[n];
    }

    /* Lazy memoisation of already computed terms. */
    static seq_t *memo      = NULL;   /* array of computed values */
    static idx_t  memo_size = 0;      /* number of terms currently stored */
    static idx_t  memo_cap  = 0;      /* allocated capacity of the array */

    /* Initialise the memo array on first use. */
    if (memo == NULL) {
        /* Allocate an initial capacity that is enough for at least 64 terms
           but will grow automatically when needed. */
        memo_cap = 64;
        if (memo_cap < n + 1)
            memo_cap = n + 1;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (memo == NULL) return 0;          /* allocation failed */
        /* Store the first nine known terms. */
        memo[0] = 1;
        memo[1] = 16;
        memo[2] = 240;
        memo[3] = 3600;
        memo[4] = 54000;
        memo[5] = 810000;
        memo[6] = 12150000;
        memo[7] = 182250000;
        memo[8] = 2733749984;
        memo_size = 9;
    }

    /* Grow the memo array if we need to compute more terms. */
    if (n >= memo_size) {
        idx_t needed = n + 1;
        if (needed > memo_cap) {
            size_t new_cap = memo_cap;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;        /* allocation failed */
            memo = tmp;
            memo_cap = new_cap;
        }
        /* Compute all missing terms up to n using the recurrence. */
        for (idx_t k = memo_size; k <= n; ++k) {
            seq_t sum = 0;
            for (idx_t i = 1; i <= 7; ++i) {
                sum += 14 * memo[k - i];
            }
            sum -= memo[k - 8];
            memo[k] = sum;
        }
        memo_size = n + 1;
    }

    return memo[n];
}
