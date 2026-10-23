/*
 * A103634  a(1) = 0; a(n) = the least k such that
 *          ((n+1)(n+2)(n+3)...(n+k))/((n-1)(n-2)(n-3)...(n-k)) is an integer
 *          (see A092935).
 * https://oeis.org/A103634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k >= 1 such that (n+1)(n+2)...(n+k) /
 *            (n-1)(n-2)...(n-k) is an integer (a(1)=0). The function
 *            iteratively updates prime exponent differences using a smallest
 *            prime factor table
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int *spf = NULL;          /* smallest prime factor */
static size_t spf_cap = 0;

static int *diff = NULL;         /* exponent difference array */
static size_t diff_cap = 0;

static void ensure_sieve(size_t limit)
{
    if (limit <= spf_cap) return;

    /* free old arrays if realloc fails later */
    free(spf);
    free(diff);

    spf_cap = limit;
    diff_cap = limit;

    spf = (int *)calloc(spf_cap + 1, sizeof(int));
    diff = (int *)calloc(diff_cap + 1, sizeof(int));

    if (!spf || !diff) return;   /* allocation failed */

    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i <= spf_cap; ++i)
        spf[i] = 0;

    for (size_t i = 2; i <= spf_cap; ++i) {
        if (spf[i] == 0) {
            spf[i] = (int)i;
            if (i * i <= spf_cap) {
                for (size_t j = i * i; j <= spf_cap; j += i)
                    if (spf[j] == 0) spf[j] = (int)i;
            }
        }
    }
}

static void update_exponent(int num, int sign, int *neg_cnt)
{
    while (num > 1) {
        int p = spf[num];
        int cnt = 0;
        while (num % p == 0) {
            num /= p;
            ++cnt;
        }
        int old = diff[p];
        int neu = old + sign * cnt;
        diff[p] = neu;
        if (old < 0 && neu >= 0) (*neg_cnt)--;
        else if (old >= 0 && neu < 0) (*neg_cnt)++;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    size_t limit = 2 * (size_t)n;
    ensure_sieve(limit);
    if (!spf || !diff) return 0;  /* fall back on zero if allocation failed */

    /* reset diff array */
    memset(diff, 0, (limit + 1) * sizeof(int));

    int neg_cnt = 0;
    for (size_t k = 1; k < (size_t)n; ++k) {
        /* incorporate (n+k) into numerator */
        update_exponent((int)(n + k), +1, &neg_cnt);
        /* incorporate (n-k) into denominator */
        update_exponent((int)(n - k), -1, &neg_cnt);

        if (neg_cnt == 0) return (seq_t)k;
    }
    /* fallback: when k = n-1 always works */
    return (seq_t)(n - 1);
}
