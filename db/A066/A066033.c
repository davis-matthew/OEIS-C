/*
 * A066033  Alternating sum of primes: a(1) = A000040(1) = 2 and a(n) = a(n-1)
 *          + A000040(n)*(-1)^n for n > 1.
 * https://oeis.org/A066033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n‑1) + A000040(n) * (-1)^n (with a(1) = 2)
 * Range:     i8 n <= 50, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime generation ------------------------------------------------- */
static uint64_t *prime_list  = NULL;   /* stored primes */
static size_t   prime_cap   = 0;       /* capacity of prime_list      */
static size_t   prime_cnt   = 0;       /* number of primes currently */

/* enlarge primes until at least 'needed' primes are stored */
static void ensure_prime_count(idx_t needed)
{
    if (prime_cnt >= (size_t)needed)
        return;

    uint64_t cand = (prime_cnt == 0) ? 2 : prime_list[prime_cnt-1] + 1;
    while (prime_cnt < (size_t)needed) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t q = prime_list[i];
            if (q * q > cand)              /* exceeded sqrt(cand) */
                break;
            if (cand % q == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
                uint64_t *tmp = (uint64_t*)realloc(prime_list,
                                                   new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_cnt++] = cand;
        }
        ++cand;
    }
}

/* ---------- sequence cache --------------------------------------------------- */
static seq_t *cache_data = NULL;   /* a(1) at index 0 */
static size_t cache_cap  = 0;       /* capacity of cache_data */
static idx_t  max_cached = 0;       /* largest n computed so far */
static seq_t  current_sum = 0;      /* a(max_cached)            */

/* ---------- A066033 ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* defined only for n ≥ 1 */

    /* have all required primes ready */
    ensure_prime_count(n);

    /* enlarge cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache_data, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache_data = tmp;
        cache_cap  = new_cap;
    }

    /* if already computed return stored value */
    if (n <= max_cached)
        return cache_data[n-1];

    /* extend cache up to n */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        seq_t p = (seq_t)prime_list[k-1];        /* k-th prime */
        if (k == 1) {
            current_sum = p;                    /* a(1) = 2              */
        } else {
            seq_t sign = ((k & 1) == 0) ? (seq_t)1 : (seq_t)-1;
            current_sum += sign * p;
        }
        cache_data[k-1] = current_sum;
    }

    max_cached = n;
    return current_sum;
}
