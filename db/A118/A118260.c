/*
 * A118260  Number of weakly carefree couples (a,b) with a,b<=n.
 * https://oeis.org/A118260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (a,b) with 1 ≤ a,b ≤ n, gcd(a,b)=1, and
 *            (a is squarefree or b is squarefree)
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static idx_t max_pre = 0;       /* largest n for which we have computed the
                                   sequence */
static seq_t *count_arr = NULL; /* a[0..max_pre], a[0] unused */
static bool *sqfree_arr = NULL; /* sqfree[i] true iff i is squarefree */

/* Rebuild everything up to new_n (called only when new_n > max_pre). */
static void recompute_up_to(idx_t new_n)
{
    /* new sizes: indices 0..new_n inclusive */
    seq_t *new_count = (seq_t *)malloc((new_n + 1) * sizeof(seq_t));
    bool *new_sqfree = (bool *)malloc((new_n + 1) * sizeof(bool));

    /* initialise squarefree array */
    for (idx_t i = 0; i <= new_n; ++i)
        new_sqfree[i] = true;            /* 1 is considered squarefree */
    /* sieve primes up to new_n */
    bool *is_prime = (bool *)malloc((new_n + 1) * sizeof(bool));
    for (idx_t i = 0; i <= new_n; ++i)
        is_prime[i] = true;
    if (new_n >= 0)
        is_prime[0] = false;
    if (new_n >= 1)
        is_prime[1] = false;
    for (idx_t i = 2; i * i <= new_n; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= new_n; j += i)
                is_prime[j] = false;
    /* mark numbers that are not squarefree */
    for (idx_t i = 2; i * i <= new_n; ++i)
        if (is_prime[i]) {
            idx_t sq = i * i;
            for (idx_t j = sq; j <= new_n; j += sq)
                new_sqfree[j] = false;
        }
    free(is_prime);

    /* compute the sequence */
    new_count[0] = 0;
    if (new_n >= 1)
        new_count[1] = 1;   /* only pair (1,1) */
    for (idx_t cur = 2; cur <= new_n; ++cur) {
        seq_t add = 0;
        bool sf_cur = new_sqfree[cur];
        for (idx_t k = 1; k < cur; ++k) {
            seq_t g = gcd((seq_t)cur, (seq_t)k);
            if (g == 1) {
                if (sf_cur || new_sqfree[k])
                    add += 2;   /* (cur,k) and (k,cur) */
            }
        }
        new_count[cur] = new_count[cur - 1] + add;
    }

    /* replace old tables */
    if (count_arr) free(count_arr);
    if (sqfree_arr) free(sqfree_arr);
    count_arr = new_count;
    sqfree_arr = new_sqfree;
    max_pre = new_n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* out of bounds, but safe */
    if (n > max_pre) recompute_up_to(n);
    return count_arr[n];
}
