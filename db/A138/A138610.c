/*
 * A138610  Nonsquarefree numbers congruent to 3 mod 4.
 * https://oeis.org/A138610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 3092, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138610: Nonsquarefree numbers congruent to 3 mod 4 (sieving) */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static idx_t *g_terms = NULL;
static size_t g_terms_size = 0;
static size_t g_terms_cap = 0;
static idx_t g_limit = 0;

/* integer square root (floor) for idx_t */
static idx_t int_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static void ensure_terms(idx_t n)
{
    if (g_terms_size >= (size_t)n)
        return;

    while (g_terms_size < (size_t)n) {
        idx_t new_limit = (g_limit == 0) ? 100 : g_limit << 1;

        /* sieve to mark non‑squarefree numbers up to new_limit */
        char *sqf = (char *)malloc((size_t)(new_limit + 1));
        if (!sqf)
            exit(1);
        for (idx_t i = 0; i <= new_limit; ++i)
            sqf[i] = 1;           /* 1 indicates squarefree initially */
        sqf[0] = 0;               /* 0 is not squarefree */

        idx_t root = int_sqrt(new_limit);
        for (idx_t p = 2; p <= root; ++p) {
            if (sqf[p]) {
                idx_t sq = p * p;
                for (idx_t j = sq; j <= new_limit; j += sq)
                    sqf[j] = 0;   /* mark multiples of p² as not squarefree */
            }
        }

        /* collect numbers that are not squarefree and ≡ 3 mod 4 */
        for (idx_t i = g_limit + 1; i <= new_limit; ++i) {
            if (!sqf[i] && (i & 3) == 3) {
                if (g_terms_size == g_terms_cap) {
                    size_t new_cap = (g_terms_cap == 0) ? 16 : g_terms_cap << 1;
                    idx_t *tmp = (idx_t *)realloc(g_terms, new_cap * sizeof(idx_t));
                    if (!tmp)
                        exit(1);
                    g_terms = tmp;
                    g_terms_cap = new_cap;
                }
                g_terms[g_terms_size++] = i;
            }
        }

        free(sqf);
        g_limit = new_limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    ensure_terms(n);
    return (seq_t)g_terms[n - 1];
}
