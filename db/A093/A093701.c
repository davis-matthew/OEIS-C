/*
 * A093701  a(n) = smallest m>a(n-1) such that 1+m*n is prime, a(1) = 1.
 * https://oeis.org/A093701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 170, u32 n <= 102, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n)=min{m>a(n-1) : 1+m*n is prime} */
/* The function memoises computed values so calls may arrive in any order. */
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdbool.h>  /* bool */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t) */

static seq_t *A_cache = NULL;   /* 0‑based array of a(k) values */
static size_t A_len   = 0;       /* number of terms currently stored */
static size_t A_cap   = 0;       /* capacity of A_cache */

/* Ensure the cache can hold at least 'size' terms. */
static void ensure_cache_capacity(size_t size)
{
    if (A_cap >= size) return;
    size_t new_cap = A_cap ? A_cap * 2 : 64;
    if (new_cap < size) new_cap = size;
    seq_t *tmp = (seq_t *)realloc(A_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();     /* memory allocation failure */
    A_cache = tmp;
    A_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return a placeholder */

    if (A_len < n) {
        /* Extend the cache if needed. */
        ensure_cache_capacity(n);

        /* Compute terms from A_len+1 up to n. */
        for (idx_t k = A_len + 1; k <= n; ++k) {
            if (k == 1) {
                A_cache[0] = (seq_t)1;      /* a(1) = 1 */
            } else {
                seq_t prev = A_cache[k - 2]; /* a(k-1) */
                seq_t m = prev + (seq_t)1;   /* start searching from a(k-1)+1 */
                seq_t sn = (seq_t)k;         /* k cast to seq_t once */

                /* Search for the smallest m > prev with 1+m*k prime. */
                while (1) {
                    seq_t candidate = m * sn + (seq_t)1;
                    if (is_prime_big(candidate)) {
                        A_cache[k - 1] = m;   /* store a(k) */
                        break;
                    }
                    m += (seq_t)1;           /* m++ is not guaranteed for arbitrary big types */
                }
            }
        }
        A_len = n;
    }

    return A_cache[n - 1];
}
