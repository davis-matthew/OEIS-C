/*
 * A080756  Numbers k such that there are infinitely many multiples of k that
 *          have exactly k divisors.
 * https://oeis.org/A080756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080756: numbers k that have infinitely many multiples with exactly k divisors.
   Equivalently, all nonsquarefree integers except 4 (i.e., every k > 1 that is divisible by
   a square of a prime, except k = 4).  The function returns the nth term (n ≥ 1). */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;   /* stored terms */
static size_t capacity = 0;   /* allocated size of cache */
static idx_t count = 0;       /* number of terms in cache */
static idx_t last_checked = 1;/* largest integer examined so far */

/* Check whether x is square‑free (no prime square divides x). */
static bool is_squarefree(unsigned long long x)
{
    for (unsigned long long p = 2; p * p <= x; ++p) {
        unsigned long long sq = p * p;
        if (x % sq == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Ensure we have computed at least n terms. */
    while (count < n) {
        unsigned long long k = (unsigned long long)(++last_checked);
        if (k == 4ULL) continue;                    /* 4 is excluded */
        if (!is_squarefree(k)) {
            if (count >= capacity) {                /* grow cache */
                size_t new_cap = capacity == 0 ? 16 : capacity * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;         /* allocation failure */
                cache = tmp;
                capacity = new_cap;
            }
            cache[count++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
