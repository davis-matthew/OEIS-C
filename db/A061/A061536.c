/*
 * A061536  a(1) = 1 and a(n) = a(n-1) + (the number of primes <= n) for n >
 *          1.
 * https://oeis.org/A061536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + pi(n), a(1) = 1 (pi(n) = number of primes ≤ n)
 * Range:     u8 n <= 36, u16 n <= 830, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Cache for the prime‑counting function pi(k).  pi_arr[k] = pi(k) */
    static idx_t *pi_arr = NULL;
    static idx_t max_sieve = 0;          /* largest k for which pi_arr is valid */

    /* Cache for the sequence values.  a_arr[k] = a(k).  a_arr[0] is unused. */
#ifdef __cplusplus
    static seq_t *a_arr = nullptr;
#else
    static seq_t *a_arr = NULL;
#endif
    static size_t a_size = 0;            /* allocated array size (index 0 unused) */
    static idx_t computed_until = 0;     /* highest n for which a_arr[n] is computed */

    if (n < 1)
        return (seq_t)0;   /* out‑of‑range value – not used in real calls */

    /* ------------------------------------------------------------
       Ensure that pi_arr is valid up to n
       ------------------------------------------------------------ */
    if (n > max_sieve) {
        idx_t limit = n;
        /* Allocate new prime table */
        idx_t *new_pi = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
        if (!new_pi) return (seq_t)0;      /* allocation failure */

        /* Sieve of Eratosthenes */
        bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
        if (!is_prime) { free(new_pi); return (seq_t)0; }

        for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
        if (limit >= 0) is_prime[0] = false;
        if (limit >= 1) is_prime[1] = false;

        for (idx_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = false;
            }
        }

        idx_t cnt = 0;
        for (idx_t i = 0; i <= limit; ++i) {
            if (i >= 2 && is_prime[i]) ++cnt;
            new_pi[i] = cnt;
        }

        free(is_prime);
        free(pi_arr);
        pi_arr = new_pi;
        max_sieve = limit;
    }

    /* ------------------------------------------------------------
       Ensure that a_arr has enough space up to n
       ------------------------------------------------------------ */
#ifdef __cplusplus
    if ((size_t)n + 1 > a_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_a = new seq_t[new_size];   /* value‑initialized */
        // Copy old contents
        if (a_arr) {
            std::copy(a_arr, a_arr + a_size, new_a);
            delete[] a_arr;
        }
        a_arr = new_a;
        a_size = new_size;
    }
#else
    if ((size_t)n + 1 > a_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_a = (seq_t *)realloc(a_arr, new_size * sizeof(seq_t));
        if (!new_a) return (seq_t)0;         /* allocation failure */
        // Zero‑initialize the new part
        for (size_t i = a_size; i < new_size; ++i) new_a[i] = (seq_t)0;
        a_arr = new_a;
        a_size = new_size;
    }
#endif

    /* ------------------------------------------------------------
       Compute sequence values up to n if they are missing
       ------------------------------------------------------------ */
    if (computed_until < n) {
        for (idx_t i = computed_until + 1; i <= n; ++i) {
            if (i == 1) {
                a_arr[1] = (seq_t)1;
            } else {
                a_arr[i] = a_arr[i - 1] + (seq_t)pi_arr[i];
            }
        }
        computed_until = n;
    }

    return a_arr[n];
}
